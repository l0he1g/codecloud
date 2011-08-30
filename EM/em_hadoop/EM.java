package em;

/*
 * input: nz.txt
 *        r   u    t    0.1
 *  distributedCache:para.txt
 *   	ri  r:v;r:v;...		#row represent p(r|i), given i
 *   	ui  u:v;u:v;...		#row represent p(u|i), given i
 *   	ti  t:v;t:v;...		#row represent p(t|j), given j
 *   	ji   j:v;j:v;...		#row represent p(i,j), given i
 */
import java.io.IOException;
import java.net.URI;
import java.util.*;

import org.apache.hadoop.filecache.DistributedCache;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.*;
import org.apache.hadoop.io.compress.GzipCodec;
import org.apache.hadoop.conf.*;
import org.apache.hadoop.util.*;
import org.apache.hadoop.mapred.*; //import org.apache.hadoop.mapred.lib.MultipleTextOutputFormat;
import org.apache.hadoop.mapred.lib.NLineInputFormat;

public class EM extends Configured implements Tool {

	enum Parameters {
		ROW_PU_I, ROW_PR_I, ROW_PT_J, ROW_PIJ, BAD
	}

	public static class Map extends MapReduceBase implements
			Mapper<LongWritable, Text, Text, Text> {
		private SparseMatrix pij = new SparseMatrix( Main.dim_i );

		private SparseMatrix pri = new SparseMatrix( Main.dim_r );

		private SparseMatrix pui = new SparseMatrix( Main.dim_u );

		private SparseMatrix ptj = new SparseMatrix( Main.dim_t );

		public void configure( JobConf job ) {			
			try {
				Path[] localFiles = DistributedCache.getLocalCacheFiles( job );
				for ( Path p : localFiles ) {
					String path = p.toString();
					// System.out.println("path: "+path);
				}
				String path = localFiles[0].toString();
				Util.read_para( path, pij, pri, pui, ptj );

				//System.out.println( "End reading distributed cache" );
			} catch ( IOException e ) {
				e.printStackTrace();
			}

		}

		public void map( LongWritable key, Text value,
				OutputCollector<Text, Text> output, Reporter reporter )
				throws IOException {
			String line = value.toString();
			String[] items = line.split( "\t" );

			try {
				int r = Integer.parseInt( items[0] );
				int u = Integer.parseInt( items[1] );
				int t = Integer.parseInt( items[2] );
				float n = Float.parseFloat( items[3] );

				// compute p=n*p(r|i)*p(u|i)*p(t|j)*p(i,j)
				SparseVector pi_r = pri.get( r );
				SparseVector pi_u = pui.get( u );
				SparseVector pj_t = ptj.get( t );

				if ( pi_r == null || pi_r.empty() || pi_u == null
						|| pi_u.empty() || pj_t == null || pj_t.empty() )
					{
					System.out.printf("[Warning]No use record.<%d,%d,%d>\n", r, u,	t );
					return;
				}
				
				float p_rut = 0;// for compute likelihood

				// compute unnormalized p(i,j|r,u,t)
				SparseMatrix pij_rut = new SparseMatrix( Main.dim_i );

				for ( int i : pi_r.indexs() ) {// for i
					float p_ri = pi_r.get( i );
					float p_ui = pi_u.get( i );
					if ( p_ri > 0 && p_ui > 0 ) {
						for ( int j : pj_t.indexs() ) {// for j
							float p_tj = pj_t.get( j );

							// compute p(i,j|r,u,t)
							float p_ij = pij.get( i, j );
							if ( p_tj > 0 && p_ij > 0 ) {
								double p_all = p_ri * p_tj * p_ui * p_ij;
								p_rut += p_all; // likelihood
								pij_rut.set( i, j, (float)p_all );
							}
						}// end j
					}
				}// end i
				
				// collect likelihood
				if(p_rut>0){
					double f = n * Math.log10( p_rut );
					output.collect( new Text( "l" ), new Text( Double.toString( f ) ) );
					reporter.setStatus( "part likelihood: " + f );
				}
				
				pij_rut.normalize_mat();
				//System.out.printf( "p(i,j|%d,%d,%d)\n", r, u, t );
				
				//reporter.setStatus( "p(i,j|r,u,t) normalized." );

				// output principle: different records represent different
				// matrix elements in a matrix share the same key and so
				// catenate them

				String tmp; // record Map output Value
				SparseVector c_tj = new SparseVector();// no business with i

				for ( int i = 0; i < pij_rut.rows(); i++ ) {
					// row may be a empty vector
					SparseVector vec = pij_rut.get( i );
					if ( vec!=null && !vec.empty() ) {
						float p_sum_j = 0;// for p(r|i),p(u|i)
						for ( int j : vec.indexs() ) {
							float p = vec.get( j )*n;

							p_sum_j += p; // for p(r|i), p(u|i)
							c_tj.add( j, p ); // for p(t|j)
						}// end j

						// collect p(i,j)
						output.collect( new Text( "j" + i ), new Text( vec
								.toString() ) );

						// collect p(r|i) for each i, keys are r1,r2,...
						tmp = String.format( "%d:%e;", r, p_sum_j );
						output.collect( new Text( "r" + i ), new Text( tmp ) );

						// collect p(u|i) for each i, keys are u1,u2,...
						tmp = String.format( "%d:%e;", u, p_sum_j );
						output.collect( new Text( "u" + i ), new Text( tmp ) );
					} 
				}// end i

				// collect p(t|j) for each j, keys are t1,t2,...
				for ( int j : c_tj.indexs() ) {
					float p = c_tj.get( j );
					if ( p > 0 ) {
						tmp = String.format( "%d:%e;", t, p );
						output.collect( new Text( "t" + j ), new Text( tmp ) );
					}
				}

			} catch ( ArrayIndexOutOfBoundsException e ) {
				System.err.println( "Bad record:" + value );
			}
		}
	}

	public static class Combine extends MapReduceBase implements
			Reducer<Text, Text, Text, Text> {
		public void reduce( Text key, Iterator<Text> values,
				OutputCollector<Text, Text> output, Reporter reporter )
				throws IOException {
			if ( key.toString().equals( "l" ) ) {
				double f = 0.0;
				while ( values.hasNext() ) {
					f += Double.parseDouble( values.next().toString() );
				}
				
				output.collect( key, new Text( Double.toString( f ) ) );
				//System.out.println( "combine likelihood: " + f );
			} else {
				SparseVector vec = new SparseVector();
				while ( values.hasNext() ) {
					String s = values.next().toString();
					vec.append( s );
				}

				String v = vec.toString();
				output.collect( key, new Text( v ) );
			}
		}
	}

	/*
	 * input k/v format: case 1: <"r1", 2:0.2;4:0.3;...> value for each r given
	 * i case 2: <"u1", 2:0.2;4:0.3;...> value for each u given i case 3: <"t1",
	 * 2:0.2;4:0.3;...> value for each t given j case 4: <"i",
	 * 1,2:0.2;2,1:0.3;...> value for each p(i,j) output k/v format: case 1:
	 * <"r1", 2:0.2;4:0.3;...> value for each r given i case 2: <"u1",
	 * 2:0.2;4:0.3;...> value for each u given i case 3: <"t1", 2:0.2;4:0.3;...>
	 * value for each t given j case 4: <"j1", 2:0.2;4:0.3;...> value for each
	 * p(i,j)
	 */

	public static class Reduce extends MapReduceBase implements
			Reducer<Text, Text, Text, Text> {

		public void reduce( Text key, Iterator<Text> values,
				OutputCollector<Text, Text> output, Reporter reporter )
				throws IOException {
			String key_s = key.toString();
			if ( key_s.equals( "l" ) ) {
				reporter.setStatus( "likelihood computing" );
				// likelihood compute
				double f = 0.0;
				while ( values.hasNext() ) {
					f += Double.parseDouble( values.next().toString() );;
				}
				
				//System.out.println( "last likelihood: " + f );
				reporter.setStatus( "last likelihood: " + f );
			} else {
				
				SparseVector vec = new SparseVector();
				while ( values.hasNext() ) {
					String s = values.next().toString();
					vec.append( s );
				}
				String v;
				
				if ( key_s.charAt( 0 ) == 'j' ) {
					// v = vec.toString( Main.min_p );
					v = vec.toString();
				} else {
					vec.normalize();
					v = vec.toString();					
					//System.out.printf( "after normalize:%s\t%s\n", key_s, v );
				}

				// increase counters
				switch ( key_s.charAt( 0 ) ) {
				case 'j':
					reporter.incrCounter( Parameters.ROW_PIJ, 1 );
					break;
				case 'r':
					reporter.incrCounter( Parameters.ROW_PR_I, 1 );
					break;
				case 'u':
					reporter.incrCounter( Parameters.ROW_PU_I, 1 );
					break;
				case 't':
					reporter.incrCounter( Parameters.ROW_PT_J, 1 );
					break;
				default:
					reporter.incrCounter( Parameters.BAD, 1 );
				}
				// collect
				if ( v.length() > 0 )
					output.collect( key, new Text( v ) );
			}
		}
	}

	public int run( String[] args ) throws Exception {

		JobConf conf = new JobConf( getConf(), EM.class );

		conf.set( "mapred.line.input.format.linespermap", "24570" );
		conf.setJobName( "em" );
		conf.setMapOutputKeyClass( Text.class );
		conf.setMapOutputValueClass( Text.class );
		conf.setOutputKeyClass( Text.class );
		conf.setOutputValueClass( Text.class );

		conf.setCompressMapOutput( true );
		conf.setMapOutputCompressorClass( GzipCodec.class );

		conf.setMapperClass( Map.class );
		conf.setCombinerClass( Combine.class );
		conf.setReducerClass( Reduce.class );
		conf.setNumReduceTasks( 18 );

		conf.setInputFormat( NLineInputFormat.class );
		// conf.setOutputFormat( EmMultipleTextOutputFormat.class );
		conf.setOutputFormat( SequenceFileOutputFormat.class );
		FileInputFormat.addInputPath( conf, Main.input );
		FileOutputFormat.setOutputPath( conf, Main.output );
		/*
		 * conf.setProfileEnabled(true);
		 * conf.setProfileParams("-agentlib:hprof=cpu=samples,heap=sites,depth=6,"
		 * + "force=n,thread=y,verbose=n,file=%s");
		 * conf.setProfileTaskRange(true, "0-2");
		 */
		FileSystem fs = FileSystem.get( conf );

		DistributedCache
				.addCacheFile( new URI( Main.para_pt.toString() ), conf );// 不可以加文件夹

		if ( fs.exists( Main.output ) ) {
			fs.delete( Main.output, true );
		}

		JobClient.runJob( conf );

		DistributedCache.purgeCache( conf );

		return 0;
	}

}
