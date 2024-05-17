package org.example;

import java.io.IOException;
import java.util.HashMap;
import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class Main {
    public static volatile boolean isRunning = true;

    public static void main(String[] args) throws InterruptedException {
        Magazine magazine = new Magazine(new HashMap<>());
        ExecutorService producers = Executors.newFixedThreadPool(3);
        producers.submit(new Producer(magazine, new Random()));
        producers.submit(new Producer(magazine, new Random()));
        producers.submit(new Producer(magazine, new Random()));

        ExecutorService consumers = Executors.newFixedThreadPool(3);
        consumers.submit(new Consumer(magazine, new Random()));
        consumers.submit(new Consumer(magazine, new Random()));
        consumers.submit(new Consumer(magazine, new Random()));

        try {
            System.in.read();
            isRunning = false;
        } catch (IOException e) {
            e.printStackTrace();
        }
        consumers.shutdown();
        if (!consumers.awaitTermination (15, TimeUnit.SECONDS ) ) {
            System.out.println (" Timeout occurred while waiting for consumers. Kill the program manually ") ;
        }
        producers.shutdown();
        if (!producers.awaitTermination (15, TimeUnit.SECONDS ) ) {
            System.out.println (" Timeout occurred while waiting for producers. Kill the program manually ") ;
        }

    }
}
