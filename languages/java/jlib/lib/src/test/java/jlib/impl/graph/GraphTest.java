package jlib.impl.graph;

import com.google.common.graph.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class GraphTest {
    @Test
    public void testDagTraversal() {
        MutableGraph<String> graph = GraphBuilder.directed().build();
        graph.addNode("A");
        graph.addNode("B");
        graph.addNode("C");

        graph.putEdge("A", "B");
        graph.putEdge("A", "C");
        graph.putEdge("A", "D");

        graph.addNode("E");

        graph.putEdge("B", "E");
        graph.putEdge("C", "E");

        graph.addNode("F");

        graph.putEdge("C", "F");
        graph.putEdge("D", "F");

        graph.addNode("G");

        graph.putEdge("E", "G");
        graph.putEdge("F", "G");

        Traverser<String> traverser = Traverser.forGraph(graph);
        Iterable<String> dependentNodes = traverser.breadthFirst("B");
        dependentNodes.forEach(System.out::println); // B, E, G

        System.out.println("-------------------------------");
        Iterable<String> parentNodes = traverser.depthFirstPreOrder("B");
        parentNodes.forEach(System.out::println);
    }

    @Test
    public void testJobTraversal() {
        ImmutableGraph<String> graph = GraphBuilder.directed().<String>immutable()
                .addNode("ALL").addNode("1").addNode("2").addNode("3").addNode("4")
                .putEdge("1", "2")
                .putEdge("ALL", "1").putEdge("ALL", "3")
                .putEdge("ALL", "4")
                .build();

        Traverser<String> traverser = Traverser.forGraph(graph);
        Iterable<String> triggeredJobs = traverser.depthFirstPreOrder("3");
        triggeredJobs.forEach(System.out::println);
    }

    @Test
    public void testRunnableTraversal() {
        Job sentinel = new Job("sentinel");
        Job job1 = new Job("1");
        Job job2 = new Job("2");
        Job job3 = new Job("3");
        Job job4 = new Job("4");

        Job orphan = new Job("orphan");

        ImmutableGraph<Job> graph = GraphBuilder.directed().<Job>immutable()
                .addNode(sentinel).addNode(job1).addNode(job2).addNode(job3).addNode(job4)
                .putEdge(job1, job2).putEdge(sentinel, job1).putEdge(sentinel, job3)
                .putEdge(sentinel, job4).build();

        Traverser<Job> traverser = Traverser.forGraph(graph);
        Iterable<Job> triggeredJobs = traverser.depthFirstPreOrder(job4);
        triggeredJobs.forEach(Job::run);

        Assertions.assertFalse(graph.nodes().contains(orphan));
    }

    @Test
    public void testMultipleDag() {
        ImmutableGraph.Builder<String> graphBuilder = GraphBuilder.directed().<String>immutable();
        graphBuilder.addNode("config-shared.yaml");
        graphBuilder.addNode("config-shared-dev.yaml");
        graphBuilder.putEdge("config-shared-dev.yaml", "config-shared.yaml");
        graphBuilder.addNode("config-shared-US.yaml");
        graphBuilder.putEdge("config-shared-US.yaml", "config-shared.yaml");
        graphBuilder.addNode("app1-US-dev.yaml");
        graphBuilder.putEdge("app1-US-dev.yaml", "config-shared-US.yaml");
        graphBuilder.putEdge("app1-US-dev.yaml", "config-shared-dev.yaml");
        graphBuilder.addNode("app2-US-dev.yaml");
        graphBuilder.putEdge("app2-US-dev.yaml", "config-shared-US.yaml");

        // add something irrelevant
        graphBuilder.addNode("something-else.yaml");
        graphBuilder.addNode("another-dep.yaml");
        graphBuilder.putEdge("another-dep.yaml", "something-else.yaml");

        // add dup nodes
        graphBuilder.addNode("app3-US-dev.yaml");
        graphBuilder.addNode("config-shared.yaml");
        graphBuilder.putEdge("app3-US-dev.yaml", "config-shared.yaml");

        ImmutableGraph<String> graph = graphBuilder.build();

        Traverser<String> traverser = Traverser.forGraph(graph);

        System.out.println("-------------------------");
        traverser.depthFirstPreOrder("config-shared.yaml").forEach(System.out::println);

        System.out.println("-------------------------");
        traverser.depthFirstPreOrder("config-shared-dev.yaml").forEach(System.out::println);

        System.out.println("-------------------------");
        traverser.depthFirstPreOrder("app1-US-dev.yaml").forEach(System.out::println);

        System.out.println("-------------------------");
        traverser.depthFirstPreOrder("app2-US-dev.yaml").forEach(System.out::println);

        System.out.println("-------------------------");
        traverser.depthFirstPreOrder("another-dep.yaml").forEach(System.out::println);

        System.out.println("-------------------------");
        System.out.println(graph.nodes());
    }

    private record Job(String name) implements Runnable {
        @Override
            public void run() {
                System.out.println(name);
            }
        }
}
