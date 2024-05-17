package org.example;

import lombok.AllArgsConstructor;

import java.util.Random;

import static org.example.Main.isRunning;

@AllArgsConstructor
public class Consumer implements Runnable{
    private final Magazine magazine;
    Random rand;
    @Override
    public void run() {
        System.out.println("Consumer has started: "+Thread.currentThread().getName());
        while(isRunning){
            String commodity = Magazine.commodities[rand.nextInt(4)];
            float randomTimeToConsume = (rand.nextFloat(12)+2)*1000;
            int randAmount = rand.nextInt(3)+1;
            System.out.println("C  --> "+Thread.currentThread().getName()+"[Consumer need: "+commodity+"](amount: "+randAmount+")\n");
            int amountConsumed = consume(commodity, randomTimeToConsume, randAmount);
            if(amountConsumed==0) System.out.println("C  --> "+Thread.currentThread().getName()+" [Consumer wanted: "+commodity+"], but it's not available");
            else  System.out.println("C  --> "+Thread.currentThread().getName()+" [Has Consumed: "+commodity+"](amount: "+amountConsumed+")");
            System.out.println(magazine+"\n");
        }
    }

    private int consume(String commodity, float time, int amount) {
        try {
            Thread.sleep((long) time);
            return magazine.deleteCommodity(commodity, amount);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }
}
