Ext.onReady(function(){
	Ext.QuickTips.init();  //表示鼠标移到上面时，显示的信息
	Ext.form.Field.prototype.msgTarget = 'side'; //当有错误时，鼠标移到最后边时才显示错误
	Ext.state.Manager.setProvider(new Ext.state.CookieProvider());//初始化Ext状态管理器，在Cookie中记录用户的操作状态，如果不启用，象刷新时就不会保存当前的状态，而是重新加载 
	Ext.BLANK_IMAGE_URL = '../../js/ext/resources/images/default/s.gif';  //设置背景图片
	
	var weekUrl="../../controller/getPhases.php?type=week";	
	var monthUrl="../../controller/getPhases.php?type=month";
	var daydataUrl="../../controller/processKeyword.php?operation=1";		//线状图表统计
	var domainUrl="../../controller/processKeyword.php?operation=2";	//topdomain数据
	var hostUrl="../../controller/processKeyword.php?operation=3";	//host数据
	var urlUrl="../../controller/processKeyword.php?operation=4";		//url数据
	var useridUrl="../../controller/processKeyword.php?operation=5";	//userid数据
	
	var myMask = new Ext.LoadMask(Ext.getBody(), {msg:"数据加载中，请耐心等待……"});
	
	var dataLoad=function(store,records,opts){
		myMask.hide();
		var chartData=[];
		var gridData=[];
		var fieldName=[];
		var params={};
		var otherSum=0;
		var isNull=false;
		for (var k=0;k<store.fields.keys.length;k++){
			fieldName[k]=store.fields.keys[k];
		}
		if(records[0].get(fieldName[0])=="no"){	//没有数据
			chartData[0]=records[0].data;
			gridData[0]={};
			gridData[0][fieldName[0]]='暂无数据';
			gridData[0][fieldName[1]]='暂无数据';
			isNull=true;
		}
		else{
			if(records.length>9){	//记录值大于9
				for(var j=9;j<records.length;j++){
					otherSum=otherSum+records[j].data.num;
					gridData[j]=records[j].data;
				}
				for(var i=0;i<9;i++){
					chartData[i]=records[i].data;
					gridData[i]=records[i].data;
				}
			}
			else{		//记录值小于9
				for(var l=0;l<records.length;l++){
					chartData[l]=records[l].data;
					gridData[l]=records[l].data;
				}
			}
		}
		switch(fieldName[0]){
			case 'domain':
				params={
					gridKeys:['domain','num']
					,chartKeys:['domain','num']
					,title:'domain'
					,xtype:Ext.ux.PiechartGrid
					,keyword:'domain'
				}
				if((!isNull)&&(otherSum>0)){
					chartData[chartData.length]={
						domain:'其它'
						,num:otherSum
					}
				}
				break;
			case 'host':
				params={
					gridKeys:['host','num']
					,chartKeys:['host','num']
					,title:'host'
					,xtype:Ext.ux.PiechartGrid
					,keyword:'host'
				}
				if((!isNull)&&(otherSum>0)){
					chartData[chartData.length]={
						host:'其它'
						,num:otherSum
					}
				}
				break;
			case 'url':
				params={
					gridKeys:['url','num']
					,chartKeys:['url','num']
					,title:'url'
					,xtype:Ext.ux.PiechartGrid
					,keyword:'url'
				}
				if((!isNull)&&(otherSum>0)){
					chartData[chartData.length]={
						url:'其它'
						,num:otherSum
					}
				}
				break;
			case 'userid':
				params={
					gridKeys:['userid','num']
					,chartKeys:['userid','num']
					,title:'userid'
					,xtype:Ext.ux.PiechartGrid
					,keyword:'userid'
				}
				if((!isNull)&&(otherSum>0)){
					chartData[chartData.length]={
						userid:'其它'
						,num:otherSum
					}
				}
				break;
			default:
				break;
		}
			
		var cdatas={
			'success':true
			,'chartData':chartData
			,'gridData':gridData
		}
		var itemPanel=Ext.util.gridChart.makeItem(cdatas,params,{
			columnWidth:0.25
			,title:'title'
			,id:params.keyword
		});
		Ext.util.gridChart.showPanel(itemPanel,piePanel);
	}
	var dataLoad2=function(store,records,opts){
		myMask.hide();
		var gridData=[];
		var params={
			gridKeys:['date','visits_num','uid_num','ip_num']
			,chartKeys:['date','visits_num','uid_num','ip_num']
			,title:'关键字统计'
			,xtype:Ext.ux.LinechartGrid
			,keyword:'keyTJ'
		}
		for(var j=0;j<records.length;j++){
			//gridData[j]=records[j].data;
			gridData[j]={
				'date':Date.parseDate(records[j].data.date,'Ymd').format('Y-m-d')
				,'visits_num':records[j].data.visits_num
				,'uid_num':records[j].data.uid_num
				,'ip_num':records[j].data.ip_num
			}
		}
		
		var cdatas={
			'success':true
			,'chartData':gridData
			,'gridData':gridData
		}
		var itemPanel=Ext.util.gridChart.makeItem(cdatas,params,{
			id:params.keyword
			,tabPosition:'top'
			,resizeTabs:true
			,tabWidth:60
		});
		Ext.util.gridChart.showPanel(itemPanel,linePanel);
	}

	var dataStore = new Ext.data.JsonStore({
        url: domainUrl,
		successProperty:"success",
        root: 'rows',
        fields: [ 
			{name: 'domain', mapping: 'domain'},
			{name: 'num', mapping: 'num',type:'float'}
		],
		listeners:{
			beforeload:function(store,opts){
				myMask.show();
			}
			,load:dataLoad
		}
		//,autoLoad:true
    });
	
	var dataStore2 = new Ext.data.JsonStore({
        url: daydataUrl,
		successProperty:"success",
        root: 'rows',
        fields: [ 
			{name: 'date', mapping: 'date'},
			{name: 'visits_num', mapping: 'visits_num',type:'int'},
			{name: 'uid_num', mapping: 'uid_num',type:'int'},
			{name: 'ip_num', mapping: 'ip_num',type:'int'}
		],
		listeners:{
			beforeload:function(store,opts){
				myMask.show();
			}
			,load:dataLoad2
		}
		//,autoLoad:true
    });
	var dataStore5 = new Ext.data.JsonStore({
        url: useridUrl,
		successProperty:"success",
        root: 'rows',
        fields: [ 
			{name: 'userid', mapping: 'userid'},
			{name: 'num', mapping: 'num',type:'float'}
		],
		listeners:{
			beforeload:function(store,opts){
				myMask.show();
			}
			,load:dataLoad
		}
		//,autoLoad:true
    });
	var dataStore4 = new Ext.data.JsonStore({
        url: urlUrl,
		successProperty:"success",
        root: 'rows',
        fields: [ 
			{name: 'url', mapping: 'url'},
			{name: 'num', mapping: 'num',type:'float'}
		],
		listeners:{
			beforeload:function(store,opts){
				myMask.show();
			}
			,load:dataLoad
		}
		//,autoLoad:true
    });
	var dataStore3 = new Ext.data.JsonStore({
        url: hostUrl,
		successProperty:"success",
        root: 'rows',
        fields: [ 
			{name: 'host', mapping: 'host'},
			{name: 'num', mapping: 'num',type:'float'}
		],
		listeners:{
			beforeload:function(store,opts){
				myMask.show();
			}
			,load:dataLoad
		}
		//,autoLoad:true
    });
	//日期store
	var monthStore = new Ext.data.JsonStore({
        url: monthUrl,
		successProperty:"success",
        root: 'rows',
        fields: [ 
			{name: 'month', mapping: 'month'},
		]
		,listeners:{
			load:function(store,records,opts){
				store.each(function(record){
					var nameStr=Date.parseDate(record.get('month'),'Ym').format('Y年m月');
					record.set('month_name',nameStr);
					record.commit();
				});
			}
		}
    });
	var weekStore = new Ext.data.JsonStore({
        url: weekUrl,
		successProperty:"success",
        root: 'rows',
        fields: [ 
			{name: 'week', mapping: 'week'},
		]
		,listeners:{
			load:function(store,records,opts){
				store.each(function(record){
					var valueStr=record.get('week').slice(0,6);
					record.set('week_value',valueStr);
					record.commit();
				});
			}
		}
    });
//组装面板
	var linePanel=new Ext.Panel({
		layout:'fit'
		,id:'linePanel'
		,anchor:'100%'
		,plain:true
		,autoScroll:true
		,border:false
		,bodyStyle:'background-color:#DFE8F6'
	});
	var piePanel=new Ext.Panel({
		layout:'column'
		,id:'piePanel'
		,anchor:'100%'
		,plain:true
		,autoScroll:true
		,border:false
		,bodyStyle:'background-color:#DFE8F6'
	});
	var centerPanel=new Ext.Panel({
		layout:'anchor'
		//,id:'centerPanel'
		,plain:true
		,autoScroll:true
		,border:false
		,bodyStyle:'background:#ffffff'
		,items:[linePanel,piePanel]
		,tbar:new Ext.ux.dateToolbar({
			queryStore:[dataStore,dataStore2,dataStore3,dataStore4,dataStore5]
			,monthStore:monthStore
			,weekStore:weekStore
			,queryItemEnable:true
			,blankText:'请输入关键字'
			,emptyText:'请输入关键字'
			,queryItemId:'word'	//查询textfield id
			,queryItemName:'word'	//查询textfield name
			,buttonText:'搜索'	//查询按钮文字
			,params:{}	//附加查询参数
			,beforeQuery:function(){
				return true;
			}
			,afterQuery:function(){
				
			}
		})
	});
	var viewport=new Ext.Viewport({
		layout:'fit'
		,items:[centerPanel]
	});
	loading_js();
});