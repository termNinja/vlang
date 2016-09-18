class Demo.HelloWorld : GLib.Object {
	public static int main(string[] args) {
		int[] a = new int[10];
		for (int i = 0; i < 10; ++i)
			a[i] = 100+i;
		for (int i = 0; i < 10; ++i)
			stdout.printf("%d ", a[i]);
		return 0;
	}
}
