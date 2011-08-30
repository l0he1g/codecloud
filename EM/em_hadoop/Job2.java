package em;

/*
 * input: /user/yxh/em/tmp
 *   	ri  r:v;r:v;...		#row represent p(r|i), given i
 *   	ui  u:v;u:v;...		#row represent p(u|i), given i
 *   	ti  t:v;t:v;...		#row represent p(t|j), given j
 *   	ji   j:v;j:v;...		#row represent p(i,j), for all i,j
 *  output:
 *    	ri  r:v;r:v;...		#row represent p(r|i), given i
 *   	ui  u:v;u:v;...		#row represent p(u|i), given i
 *   	ti  t:v;t:v;...		#row represent p(t|j), given j
 *   	i   i,j:v;i,j:v;...		#row represent p(i,j), for all i,j
 *  
 */
import java.io.IOException;
import java.util.Iterator;

import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.compress.GzipCodec;
import org.apache.hadoop.mapred.FileInputFormat;
import org.apache.hadoop.mapred.FileOutputFormat;
import org.apache.hadoop.mapred.JobClient;
import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.mapred.MapReduceBase;
import org.apache.hadoop.mapred.Mapper;
import org.apache.hadoop.mapred.OutputCollector;
import org.apache.hadoop.mapred.Reducer;
import org.apache.hadoop.mapred.Reporter;
import org.apache.hadoop.mapred.SequenceFileInputFormat;
import org.apache.hadoop.util.Tool;

public class Job2 extends Configured implements Tool {
	public static class Map extends MapReduceBase implements
			Mapper<Text, Text, Text, Text> {
		/*
		 * only care about input:
		 * 		<"j1", 2:0.2;4:0.3;...> value for each p(i,j), given i 
		 * output:
		 * 		<"i", 1|2:0.2;4:0.3;...> value for each p(i,j), given i=1 
		 */
		public void map( Text key, Text value,
				OutputCollector<Text, Text> output, Reporter reporter )
				throws IOException {
			
			String key_str = key.toString();
			if ( key_str.charAt( 0 ) == 'j' ) {	
				String s_i = key_str.substring( 1 );
				String s_v = value.toString();
				
				// sum of a SparseVector doesn't save much memory, I think
				//SparseVector vec = new SparseVector(s_v);
				//float s = vec.sum();
								
				//String tv = String.format( "%s|%.12f|%s" ,s_i, s, s_v);
				String tv = String.format( "%s|%s" ,s_i, s_v);
				
				if( s_v.length()>0 )
					output.collect( new Text( "i" ), new Text( tv ) );
			} else {
				output.collect( key, value );
			}
		}
	}

	/*
	 * only care about input:
	 * 		<"i", 1|sum of p(j,1)|2:0.2;4:0.3;...> value for each p(i,j), given i=1 
	 * output:
	 * 		<"j1", 2:0.1;4:0.15;...>  normalized value for each p(i,j), given i 
	 */
	public static class Reduce extends MapReduceBase implements
			Reducer<Text, Text, Text, NullWritable> {
		public void reduce( Text key, Iterator<Text> values,
				OutputCollector<Text, NullWritable> output, Reporter reporter )
				throws IOException {
			
			if ( key.toString().equals( "i" ) ) {// normalize p(ij)								
				SparseMatrix p_ij = new SparseMatrix(Main.dim_i);
				//float all_sum = 0;
				
				while ( values.hasNext() ) {					
					String[] items = values.next().toString().split( "\\|" );
					
					int i = Integer.parseInt( items[0] );
					///all_sum += Float.parseFloat( items[1] );
					
					p_ij.set( i, items[1] );
				}
				
				// normalize -- no need
				p_ij.normalize_mat();

				//System.out.println( "Reduce output:"+key.toString()+"\t"+p_ij.toString() );
				
				// collect each row, it will reduce memory requirement
				for( int i=0; i<p_ij.rows(); i++ ){
					SparseVector vec = p_ij.get(i);
					if( vec!=null && !vec.empty() ){
						String s_out = String.format( "j%d\t%s", i, vec.toString() );
						output.collect( new Text(s_out), NullWritable.get() );
					}else{
						System.out.printf( "[Warning!] No any j come with i:%d\n", i);
					}
				}
			} else {
				while ( values.hasNext() ) {
					String s = String.format( "%s\t%s", key.toString(), values.next().toString() );
					//System.out.println( "Reducd output:"+key.toString()+"\t"+s );
					output.collect( new Text(s), NullWritable.get() );
				}
			}
		}
	}

	public int run( String[] args ) throws Exception {	
		JobConf job = new JobConf( Job2.class );
		job.setJobName( "combine parameters files and normalize p(i,j)" );
		job.setNumReduceTasks( 1 );
		
		job.setMapOutputKeyClass( Text.class );
		job.setMapOutputValueClass( Text.class );
		job.setOutputKeyClass( Text.class );
		job.setOutputValueClass( NullWritable.class );
		job.setInputFormat( SequenceFileInputFormat.class );
		
		job.setMapperClass( Map.class );
		
		job.setReducerClass( Reduce.class);		
		
		FileInputFormat.addInputPath( job, Main.output );
		FileOutputFormat.setOutputPath( job, Main.para_dir );

		/*job.setProfileEnabled(true);
		job.setProfileParams("-agentlib:hprof=cpu=samples,heap=sites,depth=6," +
		            "force=n,thread=y,verbose=n,file=%s");
		job.setProfileTaskRange(true, "0-2");
		 */
		job.setCompressMapOutput(true);
		job.setMapOutputCompressorClass(GzipCodec.class);
		
		FileSystem fs = FileSystem.get( job );
		if ( fs.exists( Main.para_dir ) ) {
			fs.delete( Main.para_dir, true );
		}
		JobClient.runJob( job );

		return 0;
	}

}
