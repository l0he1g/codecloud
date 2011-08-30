package em;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;

public class SparseVector {
	private HashMap<Integer, Float> vec;
	
	public SparseVector() {
		// TODO Auto-generated constructor stub
		vec = new HashMap<Integer, Float>(10);
	}
	/*
	 * convert string "k1:v1;k2:v2;..." to HashMap
	 */
	public SparseVector(String s){
		String[] kvs = s.split( ";" );
		
		vec = new HashMap<Integer, Float>(kvs.length,1);
		
		for( String kv: kvs ){
			String[] items = kv.split(":");
			assert( items.length == 2 );
			
			Integer k = new Integer(items[0]);
			Float v = new Float( items[1] );
			if(vec.containsKey( k )){
				vec.put( k, vec.get( k ) + v );
			}
			else{
				vec.put( k, v );
			}
			items = null;
			kv = null;
		}
		kvs = null;
		s = null;
	}
		
	public void append(String s ){
		String[] kvs = s.split( ";" );
		for( String kv: kvs ){
			String[] items = kv.split(":");
			assert( items.length == 2 );
						
			Integer k = new Integer(items[0]);
			Float v = new Float( items[1] );
			if(vec.containsKey( k )){
				vec.put( k, vec.get( k ) + v );
			}
			else{
				vec.put( k, v );
			}
			items = null;
		}
		kvs = null;
		s = null;
	}
	
	public boolean empty(){
		return (vec.size()==0);
	}
	/*
	 * connect k,v in map by ';'
	 * output:  k1:v1;k2:v2;...;
	 */
	public String toString(){
		StringBuffer sbr = new StringBuffer(vec.size()*10);
		for( Map.Entry<Integer, Float> e: vec.entrySet() ){
			sbr.append( e.getKey() );
			sbr.append( ':' );
			sbr.append( e.getValue() );
			sbr.append( ';' );
		}
		
		return sbr.toString();
	}
	
	public String toString(float threshold){
		StringBuffer sbr = new StringBuffer(vec.size()*10);
		for( Map.Entry<Integer, Float> e: vec.entrySet() ){
			Float v = e.getValue();
				if( v> threshold ){
				sbr.append( e.getKey() );
				sbr.append( ':' );
				sbr.append( e.getValue() );
				sbr.append( ';' );
			}
		}
		if( sbr.length()==0 )
			return toString();
		
		return sbr.toString();
	}

	public void add( int i, float v){
		if( vec.containsKey( i )){
			vec.put( new Integer(i), vec.get( i )+v );
		}else{
			vec.put( new Integer(i), v );
		}
	}
	
	public int max_key(){
		int m=-1;
		for(Integer k: vec.keySet()){
			if( k>m )
				m = k.intValue();
		}
		return m;
	}
	
	public void set( int i, float v){
		vec.put( new Integer(i), new Float(v) );
	}
	
	public boolean containsKey( int i){
		return vec.containsKey( new Integer(i) );
	}
	
	public float get( int i){
		if( containsKey( i ))
			return vec.get(i);
		else
			return 0;
	}
	
	public Set<Integer> indexs(){
		return vec.keySet();
	}
	
	public float sum(){
		float s = 0;		
		for(Map.Entry<Integer, Float> e: vec.entrySet()){
			s += e.getValue();
		}
		assert( s > 0 );
		return s;
	}
	
	public void divide( float d ){
		assert( d != 0 );
		for(Map.Entry<Integer, Float> e: vec.entrySet()){
			vec.put( e.getKey(), e.getValue()/d );
		}	
	}
	
	public void normalize(){
		float s = sum();
		for(Map.Entry<Integer, Float> e: vec.entrySet()){
			vec.put( e.getKey(), e.getValue()/s );
		}
	}
	
	public void print(){
		for( Map.Entry<Integer, Float> e: vec.entrySet() ){
			System.out.printf( "%s:%s;", e.getKey(), e.getValue().toString() );
		}
		System.out.println();
	}
}
