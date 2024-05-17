package org.example;

import lombok.AllArgsConstructor;

@AllArgsConstructor
public class Consumer implements Runnable{
    private final Magazine magazine;
    @Override
    public void run() {

    }
}
