docker build . -t genomic
docker run -it --memory="4g" --name genomic genomic /bin/bash
docker rm genomic
