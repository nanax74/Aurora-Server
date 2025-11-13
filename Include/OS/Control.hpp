#pragma once

#include <vpad/input.h>
#include <queue>
#include <variant>
#include <chrono>
#include <algorithm>

namespace OS
{
    class Control
    {
    public:
        using Clock = std::chrono::steady_clock;
        using Duration = std::chrono::milliseconds;
        using TimePoint = Clock::time_point;

        // ==== 入力タイプ定義 ====
        struct ButtonInput
        {
            VPADButtons button;
            Duration remaining;
        };

        struct LeftStickInput
        {
            VPADVec2D vec;
            Duration remaining;
        };

        struct RightStickInput
        {
            VPADVec2D vec;
            Duration remaining;
        };

        using InputVariant = std::variant<ButtonInput, LeftStickInput, RightStickInput>;

    private:
        static std::queue<InputVariant> queue;
        static TimePoint lastTick;  // 前回tick()呼び出し時刻

    public:
        static bool isEmpty()
        {
            return queue.empty();
        }

        static void addButton(const uint32_t button, const uint32_t timeMs)
        {
            queue.push(ButtonInput{ static_cast<VPADButtons>(button), Duration(timeMs) });
        }

        static void addLeftStick(const float x, const float y, const uint32_t timeMs)
        {
            queue.push(LeftStickInput{ {x, y}, Duration(timeMs) });
        }

        static void addRightStick(const float x, const float y, const uint32_t timeMs)
        {
            queue.push(RightStickInput{ {x, y}, Duration(timeMs) });
        }

        static void clear()
        {
            while (!queue.empty()) queue.pop();
            
            lastTick = TimePoint{};
        }

        static void tick(VPADStatus& status)
        {
            auto now = Clock::now();
            Duration delta(0);

            if (lastTick != TimePoint{})
            {
                delta = std::chrono::duration_cast<Duration>(now - lastTick);
            }
            lastTick = now;

            if (queue.empty()) return;

            auto& front = queue.front();

            std::visit([&](auto& input)
            {
                using T = std::decay_t<decltype(input)>;
                input.remaining -= std::min(input.remaining, delta);

                if constexpr (std::is_same_v<T, ButtonInput>)
                {
                    status.hold |= input.button;
                } 
                else if constexpr (std::is_same_v<T, LeftStickInput>)
                {
                    status.leftStick.x += input.vec.x;
                    status.leftStick.y += input.vec.y;
                } 
                else if constexpr (std::is_same_v<T, RightStickInput>)
                {
                    status.rightStick.x += input.vec.x;
                    status.rightStick.y += input.vec.y;
                }
            }, front);

            // 入力が終了したら次へ
            bool expired = std::visit([](auto& input)
            {
                return input.remaining.count() <= 0;
            }, front);

            if (expired) queue.pop();
        }

        static void resetClock()
        {
            lastTick = Clock::now();
        }
    };

    // ==== staticメンバ定義 ====
    inline std::queue<Control::InputVariant> Control::queue;
    inline Control::TimePoint Control::lastTick{};
}
