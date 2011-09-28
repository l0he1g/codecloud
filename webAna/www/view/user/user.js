Ext.onReady(function(){
	Ext.QuickTips.init();  //表示鼠标移到上面时，显示的信息
	Ext.form.Field.prototype.msgTarget = 'side'; //当有错误时，鼠标移到最后边时才显示错误
	Ext.state.Manager.setProvider(new Ext.state.CookieProvider());//初始化Ext状态管理器，在Cookie中记录用户的操作状态，如果不启用，象刷新时就不会保存当前的状态，而是重新加载 
	Ext.BLANK_IMAGE_URL = '../../js/ext/resources/images/default/s.gif';  //设置背景图片
	
	var weekUrl="../../controller/getPhases.php?type=week";	//月份
	var monthUrl="../../controller/getPhases.php?type=month";	//月份
	var daydataUrl="../../controller/processUserID.php?operation=1";
	var domainUrl="../../controller/processUserID.php?operation=2";
	var hostUrl="../../controller/processUserID.php?operation=3";
	//var urlUrl="../../controller/processUserID.php?operation=4";
	var keywordsUrl="../../controller/processUserID.php?operation=8";
	
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
			records[0].set(fieldName[0],'无');
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
					,title:'域名'
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
					,title:'主机名'
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
			case 'keywords':
				params={
					gridKeys:['keywords','num']
					,chartKeys:['keywords','num']
					,title:'关键字'
					,xtype:Ext.ux.PiechartGrid
					,keyword:'keywords'
				}
				if((!isNull)&&(otherSum>0)){
					chartData[chartData.length]={
						keywords:'其它'
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
			,title:params.title
			,id:params.keyword
		});
		Ext.util.gridChart.showPanel(itemPanel,piePanel);
	}
	var dataLoad2=function(store,records,opts){
		myMask.hide();
		var gridData=[];
		var params={
			gridKeys:['date','visits_num','domain_num','host_num']
			,chartKeys:['date','visits_num','domain_num','host_num']
			,title:'统计'
			,xtype:Ext.ux.LinechartGrid
			,keyword:'keyTJ'
		}
		for(var j=0;j<records.length;j++){
			gridData[j]={
				'date':Date.parseDate(records[j].data.date,'Ymd').format('Y-m-d')
				,'visits_num':records[j].data.visits_num
				,'domain_num':records[j].data.domain_num
				,'host_num':records[j].data.host_num
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
			,tipKeys:['访问网站次数','访问域名数目','访问主机数目']
			,colName:['日期','访问网站次数','访问域名数目','访问主机数目']
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
			{name: 'domain_num', mapping: 'domain_num',type:'int'},
			{name: 'host_num', mapping: 'host_num',type:'int'}
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
        url: keywordsUrl,
		successProperty:"success",
        root: 'rows',
        fields: [ 
			{name: 'keywords', mapping: 'keywords'},
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
		//,frame:true
		,bodyStyle:'background-color:#DFE8F6'
	});
	var piePanel=new Ext.TabPanel({
		id:'piePanel'
		,anchor:'100%'
		,autoHeight:true
		,activeTab:0
		,tabPosition:'top'
		,animScroll:true
		,enableTabScroll:true
		,plain:true
		,autoScroll:true
		,border:false
		//,frame:true
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
			queryStore:[dataStore,dataStore2,dataStore3,dataStore5]
			,monthStore:monthStore
			,weekStore:weekStore
			,queryItemEnable:true
			,blankText:'请输入用户id'
			//,emptyText:'请输入用户id'
			,labelText:'用户ID:'
			,queryItemId:'uid'	//查询textfield id
			,queryItemName:'uid'	//查询textfield name
			,buttonText:'搜索用户'	//查询按钮文字
			,params:{}	//附加查询参数
			,beforeQuery:function(){
				if(Ext.isGecko){
					piePanel.removeAll(true);
					linePanel.removeAll(true);
				}
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