package com.github.dhu;

import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerConfig;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.apache.kafka.common.serialization.StringSerializer;

import java.util.Properties;

public class ProducerDemo {
    public static void main(String[] args) {
        // create producer properties
        Properties prop = new Properties();
        prop.setProperty(
                ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, // "bootstrap.servers"
                "127.0.0.1:9092");
        prop.setProperty(
                ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG, // "key.seralizer"
                StringSerializer.class.getName());
        prop.setProperty(
                ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG, // "value.seralizer"
                StringSerializer.class.getName());

        // create the producer
        KafkaProducer<String, String> producer = 
            new KafkaProducer<String, String>(prop);

        // create a producer record
        ProducerRecord<String, String> prec = 
            new ProducerRecord<String, String>("first-topic", "hello world");
        // ProducerRecord(String topic, String value);

        // send data
        producer.send(prec);

        producer.flush(); // you won't receive data without this line
        producer.close();
    }
}
