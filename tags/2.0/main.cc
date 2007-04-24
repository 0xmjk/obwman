/* 
 * okon's bandwidth manager - Simple and efficient traffic shaping solution
 * Copyright (C) 2004-2006 Michal J. Kubski
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; version 2 of the License. 
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
  
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#include <map>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include "autoconfig.h"
#include "config.h"

#include "call.h"
#include "configuration.h"
#include "util.h"

using namespace obwman;
using namespace std;

static char version_notice[] = "Copyright (C) 2006 Michal J. Kubski\n"\
"This is free software.  You may redistribute copies of it under the terms of\n"\
"the GNU General Public License <http://www.gnu.org/licenses/gpl.html>.\n"\
"There is NO WARRANTY, to the extent permitted by law.";

class bandwidth_manager {

  private:

    string OBOC;
    string OBOQ;

    configuration cfg;
    call _call;

    inline string ipt_down(string s) {
	return "-A SHAPER_DOWN " + s + "\n";
    }

    inline string ipt_up(string s) {
	return "-A SHAPER_UP " + s + "\n";
    }
    
    inline string ipt(string s) {
	return s + "\n";
    }

    void ShapeUp(string IP, int rate, int ceil, int id) {
	string i = to_string(id);
	string i1 = to_string(id + 1);
	string i2 = to_string(id + 2);
	int halfrate = int (rate / 2);

	string mark = to_string(id | 0x20000);
	string mark_omit = to_string(id | 0x30000);
	string mark_local = to_string(id | 0x40000);
	string mark_http = to_string(id | 0x50000);


	string buffer = "*mangle\n";

	/* local */

	for (vector < string >::const_iterator vi = cfg.local_ports_list.begin();
	     vi != cfg.local_ports_list.end(); ++vi) {


	    buffer += ipt_up("-p tcp -d " + cfg.ip + "/32  --dport " + (*vi) +
			     " -s " + IP + " -j MARK --set-mark " +
			     mark_local);

	    buffer += ipt_up("-p tcp -d " + cfg.ip + "/32 --dport " + (*vi) +
			     " -s " + IP + " -j RETURN");


	    buffer += ipt_up("-p udp -d " + cfg.ip + "/32  --dport " + (*vi) +
			     " -s " + IP + " -j MARK --set-mark " +
			     mark_local);

	    buffer += ipt_up("-p udp -d " + cfg.ip + "/32 --dport " + (*vi) +
			     " -s " + IP + " -j RETURN");

	}
	/* local */


	buffer += ipt_up("-p icmp -d " + cfg.ip + "/32 " +
			 " -s " + IP + " -j MARK --set-mark " +
			 mark_local);

	buffer +=
	    ipt_up("-p icmp -d " + cfg.ip + "/32 " + " -s " + IP +
		   " -j RETURN");

	/* ommited */

	for (vector < string >::const_iterator vi = cfg.ip_omit_list.begin();
	     vi != cfg.ip_omit_list.end(); ++vi) {


	    buffer += ipt_up("-d " + (*vi) +
			     " -s " + IP + " -j MARK --set-mark " +
			     mark_omit);

	    buffer += ipt_up("-d " + (*vi) + " -s " + IP + " -j RETURN");

	}

	/* world - tcp / http */


	buffer +=
	    ipt_up(" -s " + IP + " -p tcp --dport 80 -j MARK --set-mark " +
		   mark_http);

	buffer += ipt_up(" -s " + IP + " -p tcp --dport 80 -j RETURN");

	/* world - tcp / initiate connection */


	buffer += ipt_up(" -s " + IP +
			 " -p tcp -m tcp --tcp-flags ! SYN,RST,ACK ACK -j MARK --set-mark "
			 + mark_http);

	buffer += ipt_up(" -s " + IP +
			 " -p tcp -m tcp --tcp-flags ! SYN,RST,ACK ACK -j RETURN");

	/* world - tcp / ack connection */


	buffer += ipt_up(" -s " + IP +
			 " -p tcp -m tcp --tcp-flags SYN,RST,ACK ACK -m length --length :128 -j MARK --set-mark "
			 + mark_http);

	buffer += ipt_up(" -s " + IP +
			 " -p tcp -m tcp --tcp-flags SYN,RST,ACK ACK -m length --length :128 -j RETURN");

	/* world - rest */



	buffer += ipt_up(" -s " + IP + " -j MARK --set-mark " + mark);


	buffer += ipt_up(" -s " + IP + " -j RETURN");



	_call.tc(OBOC + "0 parent 1:2 classid 1:" + i + " htb rate " +
	   to_string(rate) + cfg.speed_unit + " ceil " + to_string(ceil) +
	   cfg.speed_unit + " burst " + to_string(cfg.up_burst) + " cburst 1500");


	_call.tc(OBOC + "0 parent 1:" + i + " classid 1:" + i1 + " htb rate " +
	   to_string(halfrate) + cfg.speed_unit + " ceil " + to_string(ceil) +
	   cfg.speed_unit + " burst " + to_string(cfg.up_burst) + " cburst 1500");


	_call.tc(OBOC + "0 parent 1:" + i + " classid 1:" + i2 + " htb rate " +
	   to_string(halfrate) + cfg.speed_unit + " ceil " + to_string(ceil) +
	   cfg.speed_unit + " burst " + to_string(cfg.up_burst) + " cburst 1500");


	_call.tc(OBOQ + "0 parent 1:" + i1 + " handle " + i1 +
	   ": sfq perturb 10");


	_call.tc(OBOQ + "0 parent 1:" + i2 + " handle " + i2 +
	   ": sfq perturb 10");


        tc_filt_fw("0", mark, "1:0", i1);
	
        tc_filt_fw("0", mark_http, "1:0", i2);

	tc_filt_fw("0", mark_omit, "1:0", "4");

	tc_filt_fw("0", mark_local, "1:0", "5");

	buffer += "COMMIT\n";
	
	_call.iptables_atomic(buffer);


    }

    int tc_filt_fw(string imq, string fwmark, string parent, string child) {
	return _call.tc(" filter add dev imq" + imq + " protocol ip parent " +
		  parent + " prio 1 handle " + fwmark + " fw flowid 1:" +
		  child);
    }

    void ShapeDown(string IP, int rate, int ceil, int id) {
	string i = to_string(id);
	string mark = to_string(id | 0x20000);
	string mark_omit = to_string(id | 0x30000);
	string mark_local = to_string(id | 0x40000);

	string buffer = "*mangle\n";


	/* local */


	buffer += ipt_down("-s " + cfg.ip + "/32 " +
		 " -d " + IP + " -j MARK --set-mark " + mark_local);

	buffer += ipt_down("-s " + cfg.ip + "/32 " + " -d " + IP + " -j RETURN");


	/* ommited */

	for (vector < string >::const_iterator vi = cfg.ip_omit_list.begin();
	     vi != cfg.ip_omit_list.end(); ++vi) {


	    buffer += ipt_down("-s " + (*vi) +
		     " -d " + IP + " -j MARK --set-mark " + mark_omit);

	    buffer += ipt_down("-s " + (*vi) + " -d " + IP + " -j RETURN");

	}

	/* world */


	buffer += ipt_down(" -d " + IP + " -j MARK --set-mark " + mark);

	buffer += ipt_down(" -d " + IP + " -j RETURN");



	_call.tc(OBOC + "1 parent 1:2 classid 1:" + i + " htb rate " +
	   to_string(rate) + cfg.speed_unit + " ceil " + to_string(ceil) +
	  cfg.speed_unit + " burst " + to_string(cfg.down_burst) +
	   " cburst 1500");

	_call.tc(OBOQ + "1 parent 1:" + i + " handle " + i + ": sfq perturb 10");


	tc_filt_fw("1", mark, "1:0", i);

	tc_filt_fw("1", mark_omit, "1:0", "4");

	tc_filt_fw("1", mark_local, "1:0", "5");

	buffer += "COMMIT\n";
	
	_call.iptables_atomic(buffer);

    }


  public:

    bandwidth_manager() {
	OBOC = "class add dev imq";
	OBOQ = "qdisc add dev imq";	
    }

    void conf() {
	cfg.load();
    }

    int pre() {
	int DOWN = (cfg.down_speed);
	int UP = (cfg.up_speed);
	int LAN = (cfg.lan_speed);
	int SUB = (cfg.lan_speed - cfg.down_speed);
	int SUB_DOWN = (cfg.lan_speed - cfg.down_speed);
	int SUB_UP = (cfg.lan_speed - cfg.up_speed);
	int SUB_TDOWN = int (SUB_DOWN / 3);
	int SUB_TUP = int (SUB_UP / 3);


	_call.system("modprobe imq numdevs=4");

        _call.ip(" link set dev imq0 up ");
        _call.ip(" link set dev imq1 up ");

	_call.ip(" link set dev imq0 txqueuelen 60 ");
	_call.ip(" link set dev imq1 txqueuelen 60 ");



	_call.tc(OBOQ + "0 root handle 1: htb r2q 2 default 6");

	_call.tc(OBOC + "0 parent 1: classid 1:1 htb rate " +
	   to_string(SUB_UP) + cfg.speed_unit + " ceil " +
	   to_string(SUB_UP) + cfg.speed_unit + "");

	_call.tc(OBOC + "0 parent 1: classid 1:2 htb rate " + to_string(UP) +
	   cfg.speed_unit + " ceil " + to_string(UP) + cfg.speed_unit +
	   " burst " + to_string(cfg.up_burst) + " cburst 1500");

	_call.tc(OBOC + "0 parent 1:1 classid 1:3 htb rate " +
	   to_string(SUB_TUP) + cfg.speed_unit + " ceil " +
	   to_string(SUB_UP) + cfg.speed_unit + "");

	_call.tc(OBOC + "0 parent 1:1 classid 1:4 htb rate " +
	   to_string(SUB_TUP) + cfg.speed_unit + " ceil " +
	   to_string(SUB_UP) + cfg.speed_unit + "");

	_call.tc(OBOC + "0 parent 1:1 classid 1:5 htb rate " +
	   to_string(SUB_TUP) + cfg.speed_unit + " ceil " +
	   to_string(SUB_UP) + cfg.speed_unit + "");
	// unclassified dies here

	_call.tc(OBOC +
	   "0 parent 1:1 classid 1:6 htb rate 1kbit ceil " +
	   to_string(UP) + cfg.speed_unit);

	_call.tc(OBOQ + "0 parent 1:6 handle 6: sfq perturb 10 ");

	_call.tc(OBOQ + "0 parent 1:3 handle 3: sfq perturb 10 ");

	_call.tc(OBOQ + "0 parent 1:4 handle 4: sfq perturb 10 ");

	_call.tc(OBOQ + "0 parent 1:5 handle 5: sfq perturb 10 ");

	/* Download */
	_call.tc(OBOQ + "1 root handle 1: htb r2q 2 default 6");
	_call.tc(OBOC + "1 parent 1: classid 1:1 htb rate " +
	   to_string(SUB_DOWN) + cfg.speed_unit + " ceil " +
	   to_string(SUB_DOWN) + cfg.speed_unit + "");
	_call.tc(OBOC + "1 parent 1: classid 1:2 htb rate " +
	   to_string(DOWN) + cfg.speed_unit + " ceil " +
	   to_string(DOWN) + cfg.speed_unit + " burst " +
	   to_string(cfg.down_burst) + " cburst 1500");
	_call.tc(OBOC + "1 parent 1:1 classid 1:3 htb rate " +
	   to_string(SUB_TDOWN) + cfg.speed_unit + " ceil " +
	   to_string(SUB_DOWN) + cfg.speed_unit + "");
	_call.tc(OBOC + "1 parent 1:1 classid 1:4 htb rate " +
	   to_string(SUB_TDOWN) + cfg.speed_unit + " ceil " +
	   to_string(SUB_DOWN) + cfg.speed_unit + "");
	_call.tc(OBOC + "1 parent 1:1 classid 1:5 htb rate " +
	   to_string(SUB_TDOWN) + cfg.speed_unit + " ceil " +
	   to_string(SUB_DOWN) + cfg.speed_unit + "");
	// unclassified dies here
	_call.tc(OBOC +
	   "1 parent 1:1 classid 1:6 htb rate 1kbit ceil " +
	   to_string(DOWN) + cfg.speed_unit);
	_call.tc(OBOQ + "1 parent 1:6 handle 6: sfq perturb 10 ");

	_call.tc(OBOQ + "1 parent 1:3 handle 3: sfq perturb 10 ");

	_call.tc(OBOQ + "1 parent 1:4 handle 4: sfq perturb 10 ");

	_call.tc(OBOQ + "1 parent 1:5 handle 5: sfq perturb 10 ");

	string buffer = "*mangle\n";		
	buffer += ipt(" -N SHAPER_DOWN");
	buffer += ipt(" -N SHAPER_UP");
	buffer += ipt(" -I POSTROUTING -o " + cfg.lan + " -j SHAPER_DOWN");
	buffer += ipt(" -I PREROUTING -i " + cfg.lan + " -j SHAPER_UP");
	buffer += ipt(" -I PREROUTING -i " + cfg.lan + " -j IMQ --todev 0");
	buffer += ipt(" -I POSTROUTING -o " + cfg.lan + " -j IMQ --todev 1");
	buffer += "COMMIT\n";	
	_call.iptables_atomic(buffer);
	
	cfg.ip_list.erase(cfg.ip);
    }


    int shape(string IP, int id) {
	if (cfg.ip_list.count(IP)) {
	    ShapeUp(IP, cfg.ip_list[IP].up_rate, cfg.ip_list[IP].up_ceil, id);
	    ShapeDown(IP, cfg.ip_list[IP].down_rate, cfg.ip_list[IP].down_ceil, id);
	} else {
	    ShapeUp(IP, cfg.defaults.up_rate, cfg.defaults.up_ceil, id);
	    ShapeDown(IP, cfg.defaults.down_rate, cfg.defaults.down_ceil, id);
	}
    }

    int clean() {
	_call.iptables(" -t mangle -F SHAPER_DOWN");
	_call.iptables(" -t mangle -D POSTROUTING -o " + cfg.lan + " -j SHAPER_DOWN");
	_call.iptables(" -t mangle -X SHAPER_DOWN");
	_call.iptables(" -t mangle -F SHAPER_UP");
	_call.iptables(" -t mangle -D PREROUTING -i " + cfg.lan + " -j SHAPER_UP");
	_call.iptables(" -t mangle -X SHAPER_UP");
	_call.iptables(" -t mangle -D PREROUTING -i " + cfg.lan + " -j IMQ --todev 0");
	_call.iptables(" -t mangle -D POSTROUTING -o " + cfg.lan + " -j IMQ --todev 1");
	_call.tc(" qdisc del dev imq0 root");
	_call.tc(" qdisc del dev imq1 root");
    }

    string getinterface() {
	return cfg.lan;
    }

    ~bandwidth_manager() {
    }

};

void parse_proc_net_arp(map <string, int> &hosts, unsigned int &hosts_index, bandwidth_manager &obj)
{
	string line;
        string listenOn = obj.getinterface();
	
	ifstream inFile("/proc/net/arp");
        if (!inFile) {
            cerr << "Unable to open /proc/net/arp: " << strerror(errno) << endl;
            exit (errno);  
        }
	// remove comment line
	getline(inFile, line);
	while (inFile) {
	    string ip, hwtype, flags, hwaddr, mask, dev;
	    inFile >> ip;
	    inFile >> hwtype;
	    inFile >> flags;
	    inFile >> hwaddr;
	    inFile >> mask;
	    inFile >> dev;
	    if (hwtype == "0x1" && flags != "0x0" && dev == listenOn) {
		if (!hosts[ip]) {
		    unsigned int id = hosts_index * 4;
		    obj.shape(ip, id);
		    hosts[ip] = id;
		    hosts_index++;
		}
	    }
	}
	inFile.close();
}

void arp_analyser(bandwidth_manager& obj)
{
    unsigned int hosts_index = 100;
    map < string, int > hosts;
    map < string, int >::iterator iter;

    while (1) {
	parse_proc_net_arp(hosts, hosts_index, obj);
	ofstream outFile(OBW_CLASSES_FILE);
	if (!outFile) {
            cerr << "Unable to open file (" << OBW_CLASSES_FILE << "): " << strerror(errno) << endl;
            exit (errno);  
        }

	iter = hosts.begin();
	while (iter != hosts.end()) {
	    outFile << iter->first << " " << iter->second << endl;
	    iter++;
	}
	outFile.close();
	sleep(1);
    }
}

int main(int argc, char **argv)
{
    bandwidth_manager my;
    cerr << "okon's bandwidth manager " << VERSION << endl << version_notice << endl << endl;

    if (argc < 2) {
	cerr << "usage: " << argv[0] << " (start|stop)" << endl;
	exit(-1);
    }

    string mode(argv[1]);
    int pid;
    if (mode == "stop") {
	cerr << "stopping..." << endl;
	ifstream pidFile(OBW_PID_FILE);
	if (!pidFile) {
            cerr << "Unable to open file (" << OBW_PID_FILE << "): " << strerror(errno) << endl;
            exit (errno);  
        }

	pidFile >> pid;
	pidFile.close();
	kill(pid, SIGKILL);
	my.clean();
    } else if (mode == "start") {
	cerr << "starting..." << endl;
	my.conf();
	my.pre();
	try {
	    pid = fork();
	    if (!pid) {
		sleep(10);
		arp_analyser(my);
		return 0;
	    }
	}
	catch(string err) {
	    cerr << err << endl;
	    exit(100);
	}
	ofstream pidFile(OBW_PID_FILE);
        if (!pidFile) {
            cerr << "Unable to open file (" << OBW_PID_FILE << "): " << strerror(errno) << endl;
            exit (errno);  
        }

	pidFile << pid << endl;
	pidFile.close();
    }
    cerr << "done." << endl;
    return 0;
}
