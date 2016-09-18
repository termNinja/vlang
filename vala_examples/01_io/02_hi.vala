// stdout is an object in the GLib namespace
// on which we can execute the printf function.
class Demo.HelloWorld : GLib.Object {
	public static int main(string[] args) {
		stdout.printf("Hello, World\n");
		return 0;
	}
}
