FROM gcc:14
WORKDIR /app
COPY lab1_NTA/*.cpp lab1_NTA/*.h ./
RUN g++ -std=c++17 -O2 -pthread *.cpp -o lab1
ENTRYPOINT ["./lab1"]
