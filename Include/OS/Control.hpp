#pragma once

#include <vpad/input.h>
#include <queue>
#include <chrono>
#include <algorithm>

namespace OS
{
    class Control
    {
    public:
        using Clock     = std::chrono::steady_clock;
        using Duration  = std::chrono::milliseconds;
        using TimePoint = Clock::time_point;

        // ==== 各入力タイプ ====
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

    private:
        // ==== 種類ごとに独立したキュー ====
        static std::queue<ButtonInput>     buttonQueue;
        static std::queue<LeftStickInput>  leftQueue;
        static std::queue<RightStickInput> rightQueue;

        static TimePoint lastTick;

    public:
        // ==== キューが全部空かどうか ====
        static bool isEmpty()
        {
            return buttonQueue.empty() && leftQueue.empty() && rightQueue.empty();
        }

        // ==== 追加 ====
        static void addButton(uint32_t button, uint32_t timeMs)
        {
            buttonQueue.push(ButtonInput{
                static_cast<VPADButtons>(button),
                Duration(timeMs)
            });
        }

        static void addLeftStick(float x, float y, uint32_t timeMs)
        {
            leftQueue.push(LeftStickInput{
                {x, y},
                Duration(timeMs)
            });
        }

        static void addRightStick(float x, float y, uint32_t timeMs)
        {
            rightQueue.push(RightStickInput{
                {x, y},
                Duration(timeMs)
            });
        }

        // ==== 全てクリア ====
        static void clear()
        {
            while (!buttonQueue.empty()) buttonQueue.pop();
            while (!leftQueue.empty())   leftQueue.pop();
            while (!rightQueue.empty())  rightQueue.pop();

            lastTick = TimePoint{};
        }

        // ==== 時間進行 + 入力適用 ====
        static void tick(VPADStatus& status)
        {
            auto now = Clock::now();
            Duration delta(0);

            if (lastTick != TimePoint{})
                delta = std::chrono::duration_cast<Duration>(now - lastTick);

            lastTick = now;

            // ==== BUTTON ====
            if (!buttonQueue.empty())
            {
                auto& in = buttonQueue.front();
                in.remaining -= std::min(in.remaining, delta);

                status.hold |= in.button;

                if (in.remaining.count() <= 0)
                    buttonQueue.pop();
            }

            // ==== LEFT STICK ====
            if (!leftQueue.empty())
            {
                auto& in = leftQueue.front();
                in.remaining -= std::min(in.remaining, delta);

                status.leftStick.x += in.vec.x;
                status.leftStick.y += in.vec.y;

                if (in.remaining.count() <= 0)
                    leftQueue.pop();
            }

            // ==== RIGHT STICK ====
            if (!rightQueue.empty())
            {
                auto& in = rightQueue.front();
                in.remaining -= std::min(in.remaining, delta);

                status.rightStick.x += in.vec.x;
                status.rightStick.y += in.vec.y;

                if (in.remaining.count() <= 0)
                    rightQueue.pop();
            }
        }

        static void resetClock()
        {
            lastTick = Clock::now();
        }
    };

    // ==== static メンバ定義 ====
    inline std::queue<Control::ButtonInput>     Control::buttonQueue;
    inline std::queue<Control::LeftStickInput>  Control::leftQueue;
    inline std::queue<Control::RightStickInput> Control::rightQueue;

    inline Control::TimePoint Control::lastTick{};
}
