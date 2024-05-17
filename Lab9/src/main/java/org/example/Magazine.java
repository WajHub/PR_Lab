package org.example;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.util.*;

@Getter
@Setter
@ToString
public class Magazine {
    public static final String[] commodities = new String[]{"BOOK", "LAPTOP", "PHONE", "TV"};
    public static final int MAX_CAPACITY = 4;
    private Map<String, Integer> magazine = new Hashtable<String, Integer>();
    private int size = 0;

    public Magazine(Map<String, Integer> magazine) {
        this.magazine = magazine;
        Arrays.stream(commodities).forEach(commodity -> magazine.put(commodity, 0));
    }

    public synchronized int addCommodity(String commodity){
        if(size<MAX_CAPACITY){
            magazine.put(commodity, magazine.get(commodity) + 1);
            size++;
            return 0;
        }
        return -1;
    }


}
