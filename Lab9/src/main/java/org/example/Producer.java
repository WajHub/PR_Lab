package org.example;

import lombok.AllArgsConstructor;

import java.util.Random;

import static org.example.Main.isRunning;

@AllArgsConstructor
public class Producer implements Runnable{
    private final Magazine magazine;
    Random rand ;
    @Override
    public void run() {
        System.out.println("Producer has started: "+Thread.currentThread().getName());
        while(isRunning){
            String commodity = Magazine.commodities[rand.nextInt(4)];
            float randomTimeToProduce = (rand.nextFloat(10)+2)*1000;
            System.out.println(Thread.currentThread().getName()+" [Is Producing: "+commodity+"]");
            if(produce(commodity, randomTimeToProduce)==0){
                System.out.println(Thread.currentThread().getName()+" [Has Produced: "+commodity+"]");
                System.out.println(magazine+"\n");
            }
            else{
                System.out.println(Thread.currentThread().getName()+" [Has not Produced (lack of space): "+commodity+"]");
                System.out.println(magazine+"\n");
            }
        }
    }
    private int produce(String commodity, float time){
        try {
            Thread.sleep((long) time);
            return magazine.addCommodity(commodity);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }
}

