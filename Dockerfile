FROM ubuntu:22.04 AS ctf-builder

RUN apt-get update

RUN apt-get install -y \
    make build-essential \
    gettext-base

ARG CTF_HOME="/ctf"
ARG CTF_TARGET="hello"
ARG CTF_PORT="8080"
ARG CTF_TIMEOUT="60"

ENV CTF_PATH=${CTF_HOME}/${CTF_TARGET} \
    CTF_TARGET=${CTF_TARGET} \
    CTF_PORT=${CTF_PORT} \
    CTF_TIMEOUT=${CTF_TIMEOUT}

WORKDIR ${CTF_HOME}
COPY ./*.template ./${CTF_TARGET} .
RUN make clean && \
    make && \
    envsubst < xinetd.template > /etc/xinetd.conf && \
    envsubst < run.template > /run.sh

FROM ubuntu:22.04 AS ctf-chal

RUN apt-get update

RUN apt-get install -y \
    xinetd

ARG FLAG_PATH="/flag"
ARG FLAG="SQLab{h33l0}"

RUN echo "${FLAG}" > ${FLAG_PATH} && \
    chmod 400 ${FLAG_PATH}

ARG CTF_HOME="/ctf"
ARG CTF_TARGET="hello"
ARG CTF_PATH="${CTF_HOME}/${CTF_TARGET}"

COPY --from=ctf-builder --chmod=755 ${CTF_PATH} ${CTF_PATH}
COPY --from=ctf-builder --chmod=755 /run.sh /run.sh
COPY --from=ctf-builder --chmod=400 /etc/xinetd.conf /etc/xinetd.conf

RUN chmod u+s ${CTF_PATH}

EXPOSE 8080
CMD ["xinetd", "-dontfork"]