FROM ubuntu:latest
WORKDIR /apps/data
ADD linux_system_learn.tar .
RUN tar -cvf linux_system_learn.tar linux_system_learn
ADD https://dlcdn.apache.org/tomcat/tomcat-8/v8.5.82/bin/apache-tomcat-8.5.82.tar.gz .
run tar -zxvf apache-tomcat-8.5.82.tar.gz
run mv apache-tomcat-8.5.82 tomcat-8.5.82
WORKDIR tomcat-8.5.82