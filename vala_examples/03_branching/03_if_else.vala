class Demo.HelloWorld : GLib.Object {

	public static int main(string[] args) {
		int x = 10;
		if (x > 5)
			stdout.printf(1.to_string());
		else
			stdout.printf(0.to_string());
		return 0;
	}
}
