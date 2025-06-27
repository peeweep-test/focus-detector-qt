#
# Regular cron jobs for the focus-detector-qt package.
#
0 4	* * *	root	[ -x /usr/bin/focus-detector-qt_maintenance ] && /usr/bin/focus-detector-qt_maintenance
