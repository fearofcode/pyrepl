pyrepl
======

Experimental low-latency hot-reloaded Python evaluator. Linux only at the moment.

Build with `./build.sh`. Requires Python 3.7 + libraries installed (`apt install python3-dev` or something similar).

In past versions, this was implemented in Go and ran on multiple platforms. I had trouble with duplicate events.

Basically only inotify's `IN_CLOSE_WRITE` event gives me the behavior I want.
