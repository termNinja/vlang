// stdout is an object in the GLib namespace
// on which we can execute the printf function.
class Demo.HelloWorld : GLib.Object {
	public static int main(string[] args) {
		stdout.printf("Creating Point object...\n");
		Point p = new Point(1.1, 2.2);
		p.x = 1.0;
		p.y = 2.0;
		stdout.printf(p.to_string());
		return 0;
	}
}

class Point : GLib.Object {
	public double x { get; set; default = 0.0; }
	public double y { get; set; default = 0.0; }

	public Point(double x, double y) {
		this.x = x;
		this.y = y;
	}

	public string to_string() {
		return "(" + x.to_string() + ", " + y.to_string() + ")";
	}
}
