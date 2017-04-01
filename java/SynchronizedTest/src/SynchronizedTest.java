/**
 * Created by zhuliting on 17-4-1.
 */

class Reservoir {
    private int total;

    public Reservoir(int total) {
        this.total = total;
    }

    public synchronized boolean sellTicket() {
        if (this.total > 0) {
            this.total = this.total - 1;
            return true;
        }
        else {
            return false;
        }
    }
}

class Booth extends Thread {
    private static int threadID = 0;
    private Reservoir release;
    private int count = 0;

    public Booth(Reservoir release) {
        super("id: " + (++threadID));
        this.release = release;
        this.start();
    }

    public String toString() {
        return super.getName();
    }

    public void run() {
        while (true) {
            if (this.release.sellTicket()) {
                this.count = this.count + 1;
//                System.out.println(this.getName() + ": sell 1");
                try {
                    sleep((int)(Math.random()*10));
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            } else {
//                System.out.println("not enough ticket!");
                break;
            }
        }
        System.out.println(this.getName() + ": I sold " + this.count);
    }
}

public class SynchronizedTest {
    public static void main(String[] args) {
        Reservoir reservoir = new Reservoir(100);
        Booth b1 = new Booth(reservoir);
        Booth b2 = new Booth(reservoir);
        Booth b3 = new Booth(reservoir);
    }
}
