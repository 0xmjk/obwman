# obwman
Simple tool for efficient traffic shaping for Linux 2.4/2.6 (a project from my attic); Automatically exported from code.google.com/p/obwman


obwman is a simple tool for efficient traffic shaping. It aims to give a fair share of bandwidth to each host while imposing particular rules. Its configuration is straightforward and it is almost maintenance free, as it automatically detects hosts on the network. Obwman prioritises traffic of HTTP, TCP ACK, and TCP initiate session. It runs on Linux 2.4/2.6.

### Features

* fair allocation of bandwidth
* enforce minimum and maximum speed
* support for aggregated links (uplink agnostic)
* automatic detection of hosts on the network
* support for Squid proxy
* free (available under GNU General Public License ).

### License

obwman is a free software. You may use and/or redistribute copies of it under the terms of the GNU General Public License . There is NO WARRANTY, to the extent permitted by law.

### Installation 

After downloading and unpacking the source code you need only to execute:

./configure --prefix=/usr --localstatedir=/var --sysconfdir=/etc
make
make install
Dependencies

Recent Linux kernel version (2.4.22 should make it, but 2.6.15+ were not tested thoroughly) built with IMQ, HTB, SFQ enabled.

Recent iptables (1.2.8+) and iproute2.

Binaries for iptables-restore, iptables, ip, tc should exist on your run path.

/proc/net/arp should be readable.

Obwman assumes that Squid proxy is set up as a transparent proxy and is patched against the [HIT/MISS mark patch](http://sed.pl/~mrk/qos/squid_hit_miss_mark.patch). However, use of Squid is not mandatory.


### Configuration

Configuration file obwman.conf has a clear syntax.The file consists of only one required block - general rules and definitions. The other blocks are optional.

#### Example

Here is an example of a simple configuration:

```
def     squid_port      8080
def     ip              192.168.1.1
def     lan             eth0
def     speed_unit      kbit
def     lan_speed       100000
def     down_speed      8192
def     up_speed        4096
def     down_rate       128
def     up_rate         64
def     down_ceil       1024
def     up_ceil         256
def     up_burst        1536
def     down_burst      4096


except  192.168.1.10   16      4       128     32
except  192.168.1.12   32      8       256     48
except  192.168.1.16   16      4       128     32
except  192.168.1.99   32      8       256     48


omit 10.0.6.0/24
omit 10.0.5.0/24
```

In the first block there are specified general rules and variables. The syntax is:

```
def variable value
```

where variable could be:

    squid_port - port on which the transparent Squid proxy is listening
    ip - local host ip
    lan - LAN interface name
    speed_unit - unit of measurement (all values will be specified in it)
    lan_speed - LAN speed
    down_speed - WAN download speed
    up_speed - WAN upload speed
    down_rate - guaranteed minimum of bandwidth for download (CIR)
    up_rate - guaranteed minimum of bandwidth for upload (CIR)
    down_ceil - maximum of bandwidth for download (EIR)
    up_ceil - maximum of bandwidth for upload (EIR)
    up_burst - upload burst (in bytes)
    down_burst - download burst (in bytes)

The second (optional) block covers exceptions from the general rules:

```
 except ip down_rate up_rate down_ceil up_ceil
```

Here the ip is an IP address of a host for which specific rules are assigned.

The third one add rules for hosts or subnets that are not on the LAN interface of a router, but should be considered as local i.e. those hosts which are not connected through the uplink.

```
omit address
```

The address is an IP address or a subnet address (in the CIDR notation).

