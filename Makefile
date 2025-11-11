TOPDIR ?= $(CURDIR)

include $(TOPDIR)/Rules/Setup.mk

TARGET		:=	Aurora-Server
BUILD		:=	Build
SOURCES		:=	Source ../Common/Source
DATA		:=	
INCLUDES	:=	Include ../Common/Include

include $(TOPDIR)/Rules/Flags.mk

ifneq ($(BUILD),$(notdir $(CURDIR)))

include $(TOPDIR)/Rules/Prepare.mk

else

include $(TOPDIR)/Rules/Build.mk

endif