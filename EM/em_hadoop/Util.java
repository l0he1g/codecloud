package em;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class Util {
	
	/*
	 * line: i\tr:v;r:v;...
	 * pri row is r
	 * we need transform line to a column in HashMap
	 */
	public static void read_col( String line, SparseMatrix pri){
		String[] items = line.split("\t"); 
		int i = Integer.parseInt( items[0] );
		
		String[] kvs = items[1].split( ";" );
		for( String kv: kvs){
			
			String[] rv = kv.split( ":" );
			int r = Integer.parseInt( rv[0] );
			float v = Float.parseFloat( rv[1] );
			
			pri.set( r, i, v );
		}
		items = null;
		//System.out.println();
	}
	
	/*
	 * line:i	 j:v;j:v;...
	 *  pij  row is i
	 */
	public static void read_row( String line, SparseMatrix pij){
		String[] items = line.split("\t"); 
		int i = Integer.parseInt( items[0] );
		
		if( items[1]!=null && items[1].length() > 1){			
			pij.set( i, items[1] );
		}
		items = null;
	}
	
	/*
	 * read parameters: p(i,j), p(r|i), p(u|i), p(t|j)
	 * input format:
	 *   	r  i)r:v;r:v;...		#row represent p(r|i), given i
	 *   	u  i)u:v;u:v;...		#row represent p(u|i), given i
	 *   	t  j)t:v;t:v;...		#row represent p(t|j), given j
	 *   	i  i,j:v;i,j:v;...		#row represent p(i,j), given j
	 */
	public static void read_para( String path, 
			SparseMatrix pij,		// row is i
			SparseMatrix pri,	// row is r
			SparseMatrix pui,	// row is u
			SparseMatrix ptj		// row is t	
			){
		try{
			FileReader fin = new FileReader( path );
			BufferedReader br = new BufferedReader( fin );
			
			String line;
			
			while ( ( line= br.readLine()) != null){				
				char flag = line.charAt( 0 );
				String data = line.substring( 1 ).trim();
				
				switch ( flag ) {
				case 'r':
					read_col( data, pri );
					break;
				case 'u':
					read_col( data, pui );
					break;
				case 't':
					read_col( data, ptj );
					break;
				case 'j':
					read_row( data, pij );
					break;
				default:
					System.out.println("Bad line in para file:"+line);
					break;
				}
				data = null;
				line = null;
			}//while
			br.close();
			fin.close();
		}catch( IOException e ){
			System.err.println("File Error: "+path);
			System.exit( -1 );
		}
	}	

	
	public static void main( String[] args ) {
		// test for read_mat
		/*
		String s = "1,2:0.1;1,3:0.2;2,1:0.2";
		SparseMatrix pij = new SparseMatrix(Main.dim_i);
		read_mat( s,pij );
		pij.print();
		pij.normalize_mat();
		System.out.println("After normalize");
		pij.print();
		

		// test for normalize
		/*
		String s = "2:0.1;3:0.2;1:0.2";
		System.out.printf("original s = %s\n",s);
		HashMap<String,Float> map = str2map(s);
		normalize( map );
		for( Map.Entry<String, Float> e: map.entrySet() ){			
			System.out.printf("%s:%f;", e.getKey(), e.getValue() );
		}*/
		//test for read_mat
		/*
		StringBuffer line = new StringBuffer("1,2:0.1;2,4:0.2;");
		SparseMatrix pij = new SparseMatrix();
		read_mat( line, pij );
		System.out.println("pij");
		pij.print();
		*/
		//test for read_para
		int dim_i = 68821;
		int dim_j = 49158;
		int dim_r = 74017;
		int dim_u = 501;
		int dim_t = 84373;
		
		//String local_para = "/home/xh/em/para.txt";
		String local_para = "/home/xh/mypaper/ppf/code/msn2/cluster/para/para.txt";
		SparseMatrix pij = new SparseMatrix(dim_i);
		SparseMatrix pri = new SparseMatrix(dim_r);
		SparseMatrix pui = new SparseMatrix(dim_u);
		SparseMatrix ptj = new SparseMatrix(dim_t);
		read_para( local_para , pij, pri, pui, ptj );
		System.out.println("read_para end.");
		//pri.print();
		//pui.print();
		//ptj.print();
		//pij.print();
		System.out.println("End reading");
		int I = pij.rows();
		int J = pij.cols();
		int R = pri.rows();
		int U = pui.rows();
		int T = ptj.rows();
		System.out.printf(
						"End reading distributed cache, R=%d, U=%d, T=%d, I= %d, J=%d\n",
						R, U, T, I, J );
		/*
		HashMap<String, HashMap<String,Float>> pij = new HashMap<String, HashMap<String,Float>>();
		HashMap<String, HashMap<String, Float>> pri = new HashMap<String, HashMap<String,Float>>();
		HashMap<String, HashMap<String, Float>>  pui = new HashMap<String, HashMap<String,Float>>();
		HashMap<String, HashMap<String, Float>> ptj = new HashMap<String, HashMap<String,Float>>();
		
		read_para( "test.para", pij, pri, pui, ptj );
		System.out.println("pij");
		print_mat( pij );
		System.out.println("pri");
		print_mat( pri );
		System.out.println("pui");
		print_mat( pui );
		System.out.println("ptj");
		print_mat( ptj );
		// test str2map
		/*
		String s = "1:0.1;2:0.2;1:0.3";
		System.out.printf("original s = %s\n",s);
		HashMap<String,Float> map = str2map(s);
		for( Map.Entry<String, Float> e: map.entrySet() ){			
			System.out.printf("%s : %f\n", e.getKey(), e.getValue() );
		}
		String t = map2str( map );
		System.out.printf("transformed t = %s\n", t);
		*/
		//test read_mat
		/*
		ArrayList<ArrayList<Float>> pij = Util.read_mat( "test.mat" );
		for( ArrayList<Float> row: pij ){
			for( Float v: row )
				System.out.printf("%f\t", v.floatValue() );
			System.out.println();	
		}
		*/
		
		// test read_cluster
		/*
		HashMap<Integer, HashMap<Integer, Float>> pri = Util.read_cluster( "test.map" );
		Iterator< Map.Entry<Integer, HashMap<Integer, Float>> > iter = pri.entrySet().iterator();
		while (iter.hasNext()) {
			Map.Entry<Integer, HashMap<Integer, Float>> entry = iter.next();
			Integer key = entry.getKey();
			System.out.printf( "%d\t", key.intValue() );
			
			HashMap<Integer, Float> val = entry.getValue();
			Iterator< Map.Entry<Integer, Float> > it = val.entrySet().iterator();
			while ( it.hasNext() ){
				Map.Entry<Integer, Float> e = it.next();
				Integer k = e.getKey();
				Float v = e.getValue();
				System.out.printf( "%d:%.8f,", k.intValue(), v.floatValue() );
			}
			System.out.println();
		} 
		System.out.printf("size of clusters:%d\n", pri.size()  );
		*/
	}
}
