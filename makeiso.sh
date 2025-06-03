#!/bin/bash

# Clean the live-build environment thoroughly
sudo lb clean --purge
sudo rm -rf chroot/*

# Prepare chroot environment
sudo mkdir -p chroot/etc/skel/Desktop
sudo mkdir -p chroot/usr/local/bin
sudo mkdir -p chroot/usr/share/xsessions
sudo mkdir -p chroot/etc/xdg/openbox
sudo mkdir -p chroot/etc/skel/.config/openbox
sudo mkdir -p chroot/config/package-lists
sudo mkdir -p chroot/etc/lightdm

# Copy session files explicitly
echo "[Desktop Entry]
Name=QxCentre
Comment=Minimal QxCentre Desktop Environment
Exec=/usr/local/bin/QxCentreDE
TryExec=/usr/local/bin/QxCentreDE
Type=Application" | sudo tee chroot/usr/share/xsessions/qxcentre.desktop
echo "[Desktop Entry]
Name=Fallback
Comment=Minimal QxCentre Fallback Session
Exec=openbox-session
Type=Application" | sudo tee chroot/usr/share/xsessions/fallback.desktop
sudo chmod 644 chroot/usr/share/xsessions/*.desktop

# Copy custom Openbox menu from working system
sudo cp /etc/xdg/openbox/menu.xml chroot/etc/xdg/openbox/menu.xml

# Copy unmount script and desktop entry with sudo
sudo cp ~/unmount-usb.sh chroot/usr/local/bin/
sudo chmod +x chroot/usr/local/bin/unmount-usb.sh
echo "[Desktop Entry]
Type=Application
Name=Unmount USB
Exec=/usr/local/bin/unmount-usb.sh
Icon=drive-removable-media
Terminal=false" | sudo tee chroot/etc/skel/Desktop/unmount-usb.desktop

# Configure Openbox autostart for QxCentreDE with proper shell and delay
echo "#!/bin/bash
/usr/lib/policykit-1-gnome/polkit-gnome-authentication-agent-1 &
sleep 2  # Delay to ensure X session is ready
echo 'Starting QxCentreDE at $(date)' >> /tmp/autostart.log
/usr/local/bin/QxCentreDE >> /tmp/autostart.log 2>&1 &
if [ \$? -ne 0 ]; then
    echo 'QxCentreDE failed with exit code \$? at $(date)' >> /tmp/autostart.log
    echo 'Falling back to openbox-session' >> /tmp/autostart.log
    openbox-session &
fi" | sudo tee chroot/etc/skel/.config/openbox/autostart
sudo chmod +x chroot/etc/skel/.config/openbox/autostart

# Copy QxCentreDE binary
sudo cp ~/qxcentre-de/QxCentreDE chroot/usr/local/bin/
sudo chmod +x chroot/usr/local/bin/QxCentreDE

# Configure LightDM to default to QxCentre
echo "[Seat:*]
user-session=QxCentre" | sudo tee chroot/etc/lightdm/lightdm.conf

# Update package list with explicit libwnck-3-0 and dependencies
echo "openbox lightdm libgtk-3-0 libgtk-3-dev libwnck-3-0 libwnck-3-dev libnotify4 libnotify-dev lxterminal firefox-esr mousepad pcmanfm xserver-xorg udisks2 policykit-1-gnome usbutils setserial parallel iw wireless-tools libnfc-bin libnfc-dev util-linux libmtp-dev libmtp-runtime gphoto2 nmap net-tools nfs-common iproute2 wmctrl htop vim rsync testdisk john lsof iotop tcpdump smartmontools" | sudo tee chroot/config/package-lists/my.list.chroot

# Update and build ISO
sudo apt update
sudo lb config
sudo lb build --force

# Move ISO to Isofiles directory
mkdir -p ~/Isofiles
mv live-image-amd64.hybrid.iso ~/Isofiles/qxcentre-backup-$(date +%Y-%m-%d).iso

echo "ISO created: ~/Isofiles/qxcentre-backup-$(date +%Y-%m-%d).iso"
