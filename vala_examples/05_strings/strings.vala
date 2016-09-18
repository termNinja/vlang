class Demo.HelloWorld : GLib.Object {

	public static int main(string[] args) {
		string hello = "hello";
		string world = "world";
		stdout.printf(hello + " " + world);
		return 0;
	}
}
