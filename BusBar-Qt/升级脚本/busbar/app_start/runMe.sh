cd /mnt/udisk/busbar/app_start
rm /etc/init.d/S90App
cp S90App /etc/init.d/
chmod +x /etc/init.d/*
rm /opt/start_ap
rm /opt/start_ap.sh
cp start_ap.sh /opt/
cp start_ap /opt/
cp pinyin.db /opt/
rm /opt/app
cp ../app /opt/
chmod 777 /opt/*
sync
reboot
