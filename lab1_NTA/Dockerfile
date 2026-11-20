FROM gcc:14
WORKDIR /app
COPY *.cpp *.h ./
RUN g++ -std=c++17 -O2 -o lab1 *.cpp
ENTRYPOINT ["./lab1"]
