QxCentre Brick Architecture Reference
Bricks Overview
This document lists all bricks to be developed for the QxCentre distribution, detailing their functions. This will be updated continuously and included in every ISO.
Current Bricks

USB Management Brick

Function: Handles USB mounting, unmounting, and transfer operations.
Status: Initial version with unmount-usb.sh script using udisksctl. PCManFM unmount crash persists.
Next Steps: Stabilize unmount process, add mount functionality.


Fallback Session Brick

Function: Provides a minimal desktop environment as a fallback option.
Status: Based on current live ISO, labeled “Fallback” in login menu, uses LXDE.
Next Steps: Ensure general login works in live mode.



Future Bricks

Network Management Brick: Enable Wi-Fi and wired connectivity.
Core DE Brick: Enhance the desktop environment for a robust base.
Brick Update Brick: Automate brick.md updates in ISOs.

Last Updated: June 01, 2025
