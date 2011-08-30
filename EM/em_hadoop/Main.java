package em;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.util.ToolRunner;

public class Main {
	
	/*
	public static final int dim_i = 2;
	public static final int dim_j = 1;
	public static final int dim_r = 2;
	public static final int dim_u = 2;
	public static final int dim_t = 3;
	
	public static final Path local_input = new Path( "/home/xh/em/nz.txt" );
	public static final Path local_para = new Path( "/home/xh/em/para.txt" );
	*/

	public static final int dim_i = 66552;
	public static final int dim_j = 63605;
	public static final int dim_r = 67552;
	public static final int dim_u = 410;
	public static final int dim_t = 72632;
	
	// for optimize HashMap	
	public static final Path local_input = new Path( "/home/hadoop/yxh/triples_cate.voca" );
	public static final Path local_para = new Path( "/home/hadoop/yxh/para.txt" );

	public static final float min_p = (float) 1.0e-16;//filter tiny p, save computation--unused
	public static final Path input = new Path( "/user/yxh/em/triples.txt" );
	public static final Path output = new Path( "/user/yxh/em/tmp" );   // EM job output
	public static Path para_pt = new Path( "/user/yxh/em/para/0/part-00000" );
	//开始将参数文件para.txt，上传到该目录下，名part-00000
	public static Path para_dir = new Path( "/user/yxh/em/para/0/" );
	
	public static void main( String[] args ) throws Exception {
		Configuration conf = new Configuration();
		FileSystem fs = FileSystem.get( conf );
		fs.copyFromLocalFile( Main.local_input, Main.input );
		fs.copyFromLocalFile( Main.local_para, Main.para_pt );
		
		final int max_iter = 400;
		for( int i=0;i<max_iter;i++){
			System.out.printf( "EM iter %d\n", i );
			String pt = String.format( "%s/iter%d/", "/user/yxh/em/para", i/10+1 );//间隔10次采样
			Main.para_dir = new Path( pt );   // job 2 output

			ToolRunner.run( conf , new EM(), args );
			ToolRunner.run( conf , new Job2(), args );
			
			Main.para_pt = new Path( pt + "part-00000");// EM job input
		}
		//System.exit( res );
	}
}
