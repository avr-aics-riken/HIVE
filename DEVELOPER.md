# Developer notes

## Continuous Integration

### Drone server

Assume Docker is installed in drone server(Tested on CentOS 6.5)

#### Installing drone.io

Asssume drone server has global IP.

Install drone.io as a local system(not a docker container)

For CentOS 6.x system,

    $ wget downloads.drone.io/master/drone.rpm
    $ sudo yum localinstall drone.rpm

Edit `/etc/drone/drone.toml` and fill github auth variables.
We assume drone use port `8084`

#### Run etcd 

We use `etcd` to notify build event from drone server to local host(via ssh port forwarding).

For CentOS 6.x system, Add the following line to iptables to accept access from docker container to etcd

    -A INPUT -i docker0 -j ACCEPT

Then, run `etcd` as a docker service. 

    $ docker run -d --name etcd -p 4001:4001 -p 7001:7001 coreos/etcd


### local host

Install node.js and `npm install -g shelljs`

Establish SSL port forwarding for etcd port(4001) into drone server.

    $ ssh -N -f -L 4001:localhost:8000 <drone-server-addr>

Register ssh key for K or other remote system befor running build script by using ssh-add.

    $ ssh-agent bash
    $ ssh-add /path/to/PRIVATE_KEYFILE

Go to HIVE repo and run,

    $ node ./scripts/build-watcher.js


### Run drone.io

Run `droned` and open `http://<drone-server-addr:8084`, then setup github auth(register HIVE repo to drone.io).

Then, continuous build start to work when you do `git push`


EoL.
