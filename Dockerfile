FROM ubuntu:24.04@sha256:67efaecc0031a612cf7bb3c863407018dbbef0a971f62032b77aa542ac8ac0d2

RUN apt-get update && apt-get install -y \
    qemu-system-x86 \
    socat \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY ./app /app

CMD ["socat", "tcp-l:9322,fork,reuseaddr", "EXEC:'/app/start.sh'"]