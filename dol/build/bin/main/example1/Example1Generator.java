public class Example1Generator {

  public Example1Generator() {
    //nothing to be done here
  }

  public static void main(String[] args) {
    java.util.Hashtable<String, Integer> table = new java.util.Hashtable<String, Integer>();
    System.out.println("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    System.out.println("<processnetwork xmlns=\"http://www.tik.ee.ethz.ch/~shapes/schema/PROCESSNETWORK\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" \n  xsi:schemaLocation=\"http://www.tik.ee.ethz.ch/~shapes/schema/PROCESSNETWORK     http://www.tik.ee.ethz.ch/~shapes/schema/processnetwork.xsd\" name=\"example1_flattened\">\n ");
    System.out.println("<process name=\"generator\" basename=\"generator\">");
    System.out.println("  <port name=\"1\" type=\"output\" basename=\"1\"/>");
    System.out.println("  <source location=\"generator.c\" type=\"c\"/>");
    System.out.println("</process>");
    System.out.println("<process name=\"consumer\" basename=\"consumer\">");
    System.out.println("  <port name=\"1\" type=\"input\" basename=\"1\"/>");
    System.out.println("  <source location=\"consumer.c\" type=\"c\"/>");
    System.out.println("</process>");
    System.out.println("<process name=\"square\" basename=\"square\">");
    System.out.println("  <port name=\"1\" type=\"input\" basename=\"1\"/>");
    System.out.println("  <port name=\"2\" type=\"output\" basename=\"2\"/>");
    System.out.println("  <source location=\"square.c\" type=\"c\"/>");
    System.out.println("</process>");
    System.out.println("<sw_channel name=\"C1\" type=\"fifo\" size=\"" + 10+ "\" basename=\"C1\">");
    System.out.println("  <port name=\"0\" type=\"input\" basename=\"0\"/>");
    System.out.println("  <port name=\"1\" type=\"output\" basename=\"1\"/>");
    System.out.println("</sw_channel>");
    System.out.println("<sw_channel name=\"C2\" type=\"fifo\" size=\"" + 10+ "\" basename=\"C2\">");
    System.out.println("  <port name=\"0\" type=\"input\" basename=\"0\"/>");
    System.out.println("  <port name=\"1\" type=\"output\" basename=\"1\"/>");
    System.out.println("</sw_channel>");
    System.out.println("<connection name=\"g-c\">");
    System.out.println("  <origin name=\"generator\">");
    System.out.println("    <port name=\"1\"/>");
    System.out.println("  </origin>");
    System.out.println("  <target name=\"C1\">");
    System.out.println("    <port name=\"0\"/>");
    System.out.println("  </target>");
    System.out.println("</connection>");
    System.out.println("<connection name=\"c-c\">");
    System.out.println("  <origin name=\"C2\">");
    System.out.println("    <port name=\"1\"/>");
    System.out.println("  </origin>");
    System.out.println("  <target name=\"consumer\">");
    System.out.println("    <port name=\"1\"/>");
    System.out.println("  </target>");
    System.out.println("</connection>");
    System.out.println("<connection name=\"s-c\">");
    System.out.println("  <origin name=\"square\">");
    System.out.println("    <port name=\"2\"/>");
    System.out.println("  </origin>");
    System.out.println("  <target name=\"C2\">");
    System.out.println("    <port name=\"0\"/>");
    System.out.println("  </target>");
    System.out.println("</connection>");
    System.out.println("<connection name=\"c-s\">");
    System.out.println("  <origin name=\"C1\">");
    System.out.println("    <port name=\"1\"/>");
    System.out.println("  </origin>");
    System.out.println("  <target name=\"square\">");
    System.out.println("    <port name=\"1\"/>");
    System.out.println("  </target>");
    System.out.println("</connection>");
    System.out.println("</processnetwork>");
  }

}