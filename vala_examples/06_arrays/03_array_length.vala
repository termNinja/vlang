class Demo.HelloWorld : GLib.Object {
	public static int main(string[] args) {
		int[] a = {1, 2, 3, 4, 5};
		for (int i = 0; i < a.length; ++i)
			stdout.printf("%d ", a[i]);
		return 0;
	}
}
