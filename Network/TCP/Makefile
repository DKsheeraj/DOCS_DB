all: create_namespaces create_veth_pairs assign_veth_pairs_to_namespaces configure_interfaces make_tcp_ns2_bridge server client

create_namespaces:
	sudo ip netns add tcp_ns1
	sudo ip netns add tcp_ns2
	sudo ip netns add tcp_ns3

delete_namespaces:
	sudo ip netns del tcp_ns1
	sudo ip netns del tcp_ns2
	sudo ip netns del tcp_ns3

create_veth_pairs:
	sudo ip link add tcp_veth1 type veth peer name tcp_veth2
	sudo ip link add tcp_veth3 type veth peer name veth4

assign_veth_pairs_to_namespaces:
	sudo ip link set tcp_veth1 netns tcp_ns1
	sudo ip link set tcp_veth2 netns tcp_ns2
	sudo ip link set tcp_veth3 netns tcp_ns2
	sudo ip link set veth4 netns tcp_ns3

configure_interfaces:
	sudo ip netns exec tcp_ns1 ip addr add 192.168.0.1/24 dev tcp_veth1
	sudo ip netns exec tcp_ns1 ip link set tcp_veth1 up
	sudo ip netns exec tcp_ns1 ip link set lo up
	sudo ip netns exec tcp_ns2 ip addr add 192.168.0.2/24 dev tcp_veth2
	sudo ip netns exec tcp_ns2 ip addr add 192.168.0.3/24 dev tcp_veth3
	sudo ip netns exec tcp_ns2 ip link set tcp_veth2 up
	sudo ip netns exec tcp_ns2 ip link set tcp_veth3 up
	sudo ip netns exec tcp_ns2 ip link set lo up
	sudo ip netns exec tcp_ns3 ip addr add 192.168.0.4/24 dev veth4
	sudo ip netns exec tcp_ns3 ip link set veth4 up
	sudo ip netns exec tcp_ns3 ip link set lo up

delete_interfaces:
	sudo ip netns exec tcp_ns1 ip link set tcp_veth1 down
	sudo ip netns exec tcp_ns2 ip link set tcp_veth2 down
	sudo ip netns exec tcp_ns2 ip link set tcp_veth3 down
	sudo ip netns exec tcp_ns3 ip link set veth4 down
	sudo ip netns exec tcp_ns1 ip link del tcp_veth1
	sudo ip netns exec tcp_ns2 ip link del tcp_veth3

delete_bridge:
	sudo ip netns exec tcp_ns2 brctl delif br0 tcp_veth2
	sudo ip netns exec tcp_ns2 brctl delif br0 tcp_veth3
	sudo ip netns exec tcp_ns2 ip link set br0 down
	sudo ip netns exec tcp_ns2 brctl delbr br0


delete_all: delete_bridge delete_interfaces delete_namespaces

make_tcp_ns2_bridge:
	sudo ip netns exec tcp_ns2 brctl addbr br0
	sudo ip netns exec tcp_ns2 brctl addif br0 tcp_veth2
	sudo ip netns exec tcp_ns2 brctl addif br0 tcp_veth3
	sudo ip netns exec tcp_ns2 ip link set br0 up

test_tcp_ns1:
	sudo ip netns exec tcp_ns1 ping -c 3 192.168.0.1
	sudo ip netns exec tcp_ns1 ping -c 3 192.168.0.4

test_tcp_ns3:
	sudo ip netns exec tcp_ns3 ping -c 3 192.168.0.1
	sudo ip netns exec tcp_ns3 ping -c 3 192.168.0.4

test: test_tcp_ns1 test_tcp_ns3

show_tables:
	sudo ip netns exec tcp_ns1 route
	sudo ip netns exec tcp_ns2 route
	sudo ip netns exec tcp_ns3 route

server: server.c
	gcc -o server server.c

client: client.c
	gcc -o client client.c

clean:
	rm -f server client

run_server:
	sudo ip netns exec tcp_ns3 ./server

run_client:
	sudo ip netns exec tcp_ns1 ./client

