#!/bin/sh

export HOME=$(pwd)
export LD_LIBRARY_PATH=$HOME/lib:${LD_LIBRARY_PATH:-"/usr/lib"}
exec $HOME/bin/htmlbutcher-demo


