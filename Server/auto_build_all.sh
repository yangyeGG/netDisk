#########################################################################
# File Name: Build.sh
# Author: yy
# mail: yy@ityy520.com
# Created Time: Wed 09 Jan 2019 03:29:51 PM CST
#########################################################################
#!/bin/bash
cd server_db
make
cd ../server_main
make
