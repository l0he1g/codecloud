package em;

import java.util.ArrayList;

public class SparseMatrix {
	private ArrayList<SparseVector> mat = null ;
	
	private SparseMatrix(){}
	
	public SparseMatrix(int dim){
		mat = new ArrayList<SparseVector>(dim);
		for( int c=0;c<dim;c++){
			mat.add( null );
		}
	}
	
	// set a column by a string
	// s format 2:0.2;1:0.3;
	public void set( int i, String s){
		assert(i<mat.size());
		SparseVector c_vec = new SparseVector( s );	
		mat.set( i, c_vec );
	}
	
	
	// allocate on set
	public void set( int i, int j, float v){		
		assert(i<mat.size());
		SparseVector vec = mat.get( i );
		if( vec == null ){
			vec = new SparseVector();
			mat.set( i, vec );
		}
		vec.set( j, v );
	}
	
	public SparseVector get( int i){
		return mat.get(i);		
	}
	
	public int rows(){
		return mat.size();
	}
	
	// return max column value
	public int cols(){
		int max_c = -1;
		for(SparseVector vec: mat ){
			int tmp = 0;
			if(vec!=null)
				tmp = vec.max_key();
				if(tmp>max_c)
					max_c = tmp;
		}
		return max_c+1;
	}
	
	public float get( int i, int j){
		SparseVector vec = mat.get(i);
		if( vec!=null && vec.containsKey( j ) )
			return mat.get(i).get(j);
		else
			return 0;
	}
	
	public void normalize_row(){
		for(SparseVector vec: mat){
			if(vec!=null)
				vec.normalize();
		}
	}
	
	public float sum(){
		float s = 0;
		for(SparseVector vec: mat){
			if(vec!=null)
				s += vec.sum();
		}
		
		assert( s > 0 );
		return s;
	}
	
	public void normalize_mat(){
		float s = sum();
		
		for( SparseVector vec: mat){
			if( vec!=null && !vec.empty())
				vec.divide( s );
		}
	}
	
	public void print(){
		System.out.println( "rows of mat: " + mat.size() );
		for(int i=0;i<mat.size();i++){
			System.out.printf( "row %d:\t", i);
			SparseVector vec = mat.get(i);
			if( vec!=null)
				vec.print();
		}		
	}
	public static void main( String[] args ) {
		SparseMatrix pij = new SparseMatrix();
		pij.set( 1, 2, (float)0.1 );
		System.out.println(pij.get( 1, 2 ));
		pij.print();
		
		System.out.println("--------------");		
		pij.set(4, "2:0.2;1:0.3;" );
		System.out.println(pij.get(4).toString());
		
		pij.print();
	}
}
