#sudo docker run -d -p 8080:8080 -p 55555:55555 -p 9000:9000 -p 80:80 --shm-size=2g --env username_admin_globalaccesslevel=admin --env username_admin_password=admin --name=solace-dev solace/solace-pubsub-standard
sudo docker run -d -p 8080:8080 -p 55555:55555 -p 8008:8008 -p 1883:1883 -p 8000:8000 -p 5672:5672 -p 9000:9000 -p 2222:2222 --shm-size=2g --env username_admin_globalaccesslevel=admin --env username_admin_password=admin --name=solace-dev solace/solace-pubsub-standard

# to find the container id, run sudo docker ps
# to stop, run sudo docker stop <container-id>

# to show all docker container, run sudo docker ps -a, it shows exited ones
# use sudo docker rm <container-id> to remove them
