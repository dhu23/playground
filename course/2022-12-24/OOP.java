import java.util.*;


public class OOP {

    // inheritance hierarchy
    // interface
    

    public interface Runner {
        void run();
    }

    public interface Swimmer {
        void swim();
    }

    public interface Flyer {
        void fly();
    }

    public interface Trainnable {
        void train();
        void receiveCommand(String command);
    }

    public static abstract class Animal {
        private String species;
        private int lifeSpan;

        public Animal(String species, int lifeSpan) {
            this.species = species;
            this.lifeSpan = lifeSpan;
        }

        public int getLifeSpan() { return lifeSpan; }

        public abstract void forage();
        public abstract void reproduce();
    }

    public static abstract class Mammal extends Animal {
        public Mammal(String species, int lifeSpan) {
            super(species, lifeSpan);
        }

        public boolean isWarmblooded() {
            return true;
        }
        public void feedOffsprings() {
            System.out.println("by feeding them milk");
        }
    }

    public static abstract class Bird extends Animal implements Runner {
        public Bird(String species, int lifeSpan) {
            super(species, lifeSpan);
        }

        public boolean isWarmblooded() {
            return true;
        }
    }

    // Dog subclass/derived class
    // Mammal superclass/base class

    public static class Dog extends Mammal implements Runner, Swimmer, Trainnable {
        public Dog(String species, int lifeSpan) {
            super(species, lifeSpan);
        }

        public void bark() {
            System.out.println("woof");
        }

        @Override
        public void run() {
            System.out.println("with 4 legs");
        }

        @Override
        public void train() {
            System.out.println("10 min training per day with food");
        }

        @Override
        public void receiveCommand(String command) {
            if (command.equals("sit")) {
            }
        }

        @Override
        public void swim() {
            System.out.println("dog paddle");
        }

        @Override
        public void reproduce() {
            System.out.println("by giving birth to 3-4 puppies");
        }

        @Override
        public void forage() {
            System.out.println("Sniff around and dig for bones");
        }
    }

    public static class GoldenRetriever extends Dog {
        public GoldenRetriever(String species, int lifeSpan) {
            super(species, lifeSpan);
        }

        @Override
        public void forage() {
            System.out.println("I beg and get what I want");
        }
    }

    public static class Parrot extends Bird implements Flyer, Runner {
        public Parrot(String species, int lifeSpan) {
            super(species, lifeSpan);
        }
        @Override
        public void reproduce() {
            System.out.println("by laying eggs");
        }

        @Override
        public void forage() {
            System.out.println("fly around and look for worms");
        }

        @Override
        public void fly() {
            System.out.println("spread the wings and enjoy");
        }

        @Override
        public void run() {
            System.out.println("with my two claws");
        }
    }

    public static void feedAnimals(List<Animal> animals) {
        for (Animal animal : animals) {
            animal.forage();
            System.out.println(animal.getLifeSpan());
        }
    }

    public static class Robot implements Runner {
        @Override
        public void run() {
            System.out.println("Like a human!");
        }
    }

    public static void everyoneRun(List<Runner> runners) {
        for (Runner runner : runners) {
            runner.run();
        }
    }

    public static void main(String[] args) {

        Dog myDog = new Dog("Dog", 12);
        Parrot myParrot = new Parrot("Parrot", 5);
        GoldenRetriever myGolden = new GoldenRetriever("GoldenRetriever", 12);

        List<Animal> animals = new ArrayList<>();
        animals.add(myDog);
        animals.add(myGolden);
        animals.add(myParrot);

        feedAnimals(animals);

        List<Runner> runners = new ArrayList<>();
        runners.add(myDog);
        runners.add(myGolden);
        runners.add(myParrot);
        runners.add(new Robot());
        everyoneRun(runners);
    }
}
