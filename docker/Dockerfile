FROM phusion/baseimage:latest

RUN add-apt-repository ppa:git-core/ppa
RUN apt-get update && \
    apt-get install -y git wget sudo make && \
    apt-get clean

RUN curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.deb.sh | bash && \
    rm /usr/local/bin/git-lfs || true && \
    rm /usr/bin/git-lfs || true  && \
    apt-get install -y git-lfs && \
    /usr/bin/git-lfs install && \
    apt-get install -y g++-multilib && \
    apt-get install -y libstdc++6 && \
    apt-get install -y clang

ADD . /app/

ARG PUID=1000
ARG PGID=1000
RUN groupadd -g $PGID zefiros && \
    useradd -u $PUID -g zefiros -m zefiros

# ssh:
ADD insecure_id_rsa /tmp/id_rsa 
ADD insecure_id_rsa.pub /tmp/id_rsa.pub 

ARG INSTALL_WORKSPACE_SSH=false
ENV INSTALL_WORKSPACE_SSH ${INSTALL_WORKSPACE_SSH}

RUN if [ ${INSTALL_WORKSPACE_SSH} = true ]; then \
    rm -f /etc/service/sshd/down && \
    cat /tmp/id_rsa.pub >> /root/.ssh/authorized_keys \
        && cat /tmp/id_rsa.pub >> /root/.ssh/id_rsa.pub \
        && cat /tmp/id_rsa >> /root/.ssh/id_rsa \
        && rm -f /tmp/id_rsa* \
        && chmod 600 /root/.ssh/authorized_keys /root/.ssh/id_rsa.pub \
    && chmod 400 /root/.ssh/id_rsa \
;fi

# install ZPM:
RUN wget -O - https://raw.githubusercontent.com/Zefiros-Software/ZPM/master/script/install-zpm.sh | bash

WORKDIR /app/