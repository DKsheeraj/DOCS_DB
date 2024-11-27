PKG_CONFIG_PATH=/usr/lib64/pkgconfig:/usr/local/lib64/pkgconfig:/usr/lib/pkgconfig:/usr/local/lib/x86_64-linux-gnu/pkgconfig # change this to your pkgconfig path
FF_PATH=/home/mokshith/data/f-stack# change this to your f-stack path

ifneq ($(shell pkg-config --exists libdpdk && echo 0),0)
$(error "No installation of DPDK found, maybe you should export environment variable `PKG_CONFIG_PATH`")
endif

PKGCONF ?= pkg-config

CFLAGS += -O0 -g -gdwarf-2 $(shell $(PKGCONF) --cflags libdpdk)

LIBS+= $(shell $(PKGCONF) --static --libs libdpdk)
LIBS+= -L$(FF_PATH)/lib -Wl,--whole-archive,-lfstack,--no-whole-archive
LIBS+= -Wl,--no-whole-archive -lrt -lm -ldl -lcrypto -pthread -lnuma

all: create_namespaces create_veth_pairs assign_veth_pairs_to_namespaces configure_interfaces make_dpdk_ns2_bridge server client setup

create_namespaces:
	sudo ip netns add dpdk_ns1
	sudo ip netns add dpdk_ns2
	sudo ip netns add dpdk_ns3

delete_namespaces:
	sudo ip netns del dpdk_ns1
	sudo ip netns del dpdk_ns2
	sudo ip netns del dpdk_ns3

create_veth_pairs:
	sudo ip link add dpdk_veth1 type veth peer name dpdk_veth2
	sudo ip link add dpdk_veth3 type veth peer name dpdk_veth4

assign_veth_pairs_to_namespaces:
	sudo ip link set dpdk_veth1 netns dpdk_ns1
	sudo ip link set dpdk_veth2 netns dpdk_ns2
	sudo ip link set dpdk_veth3 netns dpdk_ns2
	sudo ip link set dpdk_veth4 netns dpdk_ns3

configure_interfaces:
	sudo ip netns exec dpdk_ns1 ip addr add 192.168.0.1/24 dev dpdk_veth1
	sudo ip netns exec dpdk_ns1 ip link set dpdk_veth1 up
	sudo ip netns exec dpdk_ns1 ip link set lo up
	sudo ip netns exec dpdk_ns2 ip addr add 192.168.0.2/24 dev dpdk_veth2
	sudo ip netns exec dpdk_ns2 ip addr add 192.168.0.3/24 dev dpdk_veth3
	sudo ip netns exec dpdk_ns2 ip link set dpdk_veth2 up
	sudo ip netns exec dpdk_ns2 ip link set dpdk_veth3 up
	sudo ip netns exec dpdk_ns2 ip link set lo up
	sudo ip netns exec dpdk_ns3 ip addr add 192.168.0.4/24 dev dpdk_veth4
	sudo ip netns exec dpdk_ns3 ip link set dpdk_veth4 up
	sudo ip netns exec dpdk_ns3 ip link set lo up

delete_interfaces:
	sudo ip netns exec dpdk_ns1 ip link set dpdk_veth1 down
	sudo ip netns exec dpdk_ns2 ip link set dpdk_veth2 down
	sudo ip netns exec dpdk_ns2 ip link set dpdk_veth3 down
	sudo ip netns exec dpdk_ns3 ip link set dpdk_veth4 down
	sudo ip netns exec dpdk_ns1 ip link del dpdk_veth1
	sudo ip netns exec dpdk_ns2 ip link del dpdk_veth3

delete_bridge:
	sudo ip netns exec dpdk_ns2 brctl delif br0 dpdk_veth2
	sudo ip netns exec dpdk_ns2 brctl delif br0 dpdk_veth3
	sudo ip netns exec dpdk_ns2 ip link set br0 down
	sudo ip netns exec dpdk_ns2 brctl delbr br0


delete_all: delete_bridge delete_interfaces delete_namespaces

make_dpdk_ns2_bridge:
	sudo ip netns exec dpdk_ns2 brctl addbr br0
	sudo ip netns exec dpdk_ns2 brctl addif br0 dpdk_veth2
	sudo ip netns exec dpdk_ns2 brctl addif br0 dpdk_veth3
	sudo ip netns exec dpdk_ns2 ip link set br0 up

test_dpdk_ns1:
	sudo ip netns exec dpdk_ns1 ping -c 3 192.168.0.1
	sudo ip netns exec dpdk_ns1 ping -c 3 192.168.0.4

test_dpdk_ns3:
	sudo ip netns exec dpdk_ns3 ping -c 3 192.168.0.1
	sudo ip netns exec dpdk_ns3 ping -c 3 192.168.0.4

test: test_dpdk_ns1 test_dpdk_ns3

show_tables:
	sudo ip netns exec dpdk_ns1 route
	sudo ip netns exec dpdk_ns2 route
	sudo ip netns exec dpdk_ns3 route

server:
	$(CC) $(CFLAGS) -o server server.c $(LIBS)

client:
	gcc -o client client.c

clean:
	rm -f server client

run_server:
	sudo ip netns exec dpdk_ns3 ./server --conf ./config.ini --proc-type=primary --proc-id=0

run_client:
	sudo ip netns exec dpdk_ns1 ./client

setup:
	sudo mkdir -p /mnt/huge
	sudo mount -t hugetlbfs none /mnt/huge
	echo 1024 | sudo tee /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages

info:
	cat /proc/meminfo | grep Huge

reassign:
	sudo $(FF_PATH)/dpdk/usertools/dpdk-devbind.py -b iwlwifi 0000:00:14.3


