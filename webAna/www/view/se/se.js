Ext.onReady(function(){
	Ext.QuickTips.init();  //表示鼠标移到上面时，显示的信息
	Ext.form.Field.prototype.msgTarget = 'side'; //当有错误时，鼠标移到最后边时才显示错误
	Ext.state.Manager.setProvider(new Ext.state.CookieProvider());//初始化Ext状态管理器，在Cookie中记录用户的操作状态，如果不启用，象刷新时就不会保存当前的状态，而是重新加载 
	Ext.BLANK_IMAGE_URL = '../../js/ext/resources/images/default/s.gif';  //设置背景图片

	var weekUrl="../../controller/getPhases.php?type=week";	
	var monthUrl="../../controller/getPhases.php?type=month";
	
	var chartUrl='../../controller/processSe.php?operation=40';	//上面饼图数据
	var leftGridUrl='../../controller/processSe.php?operation=41';	//左侧表格数据
	var rightGridUrl='../../controller/processSe.php?operation=22';	//右侧表格数据
	
	var dataStore1=new Ext.data.Store({
		proxy: new Ext.data.HttpProxy({url:chartUrl}),	//本地分页
		reader:new Ext.data.JsonReader({
			totalProperty:'results'
			,root:'rows'
		},new Ext.data.Record.create([
			{name:'engine',mapping:'engine'}
			,{name:'num',mapping:'num',type:'int'}
		]))
	});
	var dataStore2=new Ext.data.Store({
		proxy: new Ext.data.HttpProxy({url:leftGridUrl}),	//本地分页
		reader:new Ext.data.JsonReader({
			totalProperty:'results'
			,root:'rows'
		},new Ext.data.Record.create([
			{name:'word',mapping:'word'}
			,{name:'google',mapping:'google'}
			,{name:'baidu',mapping:'baidu'}
			,{name:'others',mapping:'others'}
			,{name:'sum',mapping:'sum'}
		]))
	/* 	,listeners:{
			'load':function(store,records,opts){
				store.each(function(record){
					record.set('sum',record.get('google')+record.get('baidu')+record.get('others'));
					record.commit();
				});
			}
		} */
	});
	var data3={rows:[{
		host:'点击左侧表格各行'
		,num:'点击左侧表格各行'
	}]}

	var dataStore3=new Ext.data.Store({
		//proxy: new Ext.data.MemoryProxy(data2),
		proxy: new Ext.data.HttpProxy({url:rightGridUrl}),	//本地分页
		reader:new Ext.data.JsonReader({
			totalProperty:'results'
			,root:'rows'
		},new Ext.data.Record.create([
			{name:'主机名',mapping:'host'}
			,{name:'访问次数',mapping:'num'}
		]))
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
	var panelport=new Ext.Panel({
		//renderTo:Ext.getBody()
		autoScroll:true
		,border:false
		,bodyStyle:'background:#ffffff'
		,layout:'fit'
		,items:[{
			xtype:'piechartgrid2'
			,height:953
			,leftGridStore:dataStore2
			,leftColNames:['关键字','google','baidu','其它','总和']
			,rightGridStore:dataStore3
			,rightColNames:['主机名','点击率']
			,chartStore:dataStore1
			,rightGridTitle:'搜索关键词访问的网站排名'
			,leftGridTitle:'搜索关键词排名'
			,loadMask:true
			,pageSize:18
		}]
		,tbar:new Ext.ux.dateToolbar({
			queryStore:[dataStore1,dataStore2]
			,monthStore:monthStore
			,weekStore:weekStore
			,queryItemId:'host'	//查询textfield id
			,queryItemName:'host'	//查询textfield name
			,buttonText:'确定'	//查询按钮文字
			,selectQueryEnabled:true	//选择时间下拉框后即触发相应事件
			,buttonPosition:'right'
			,queryItemEnable:false
			,params:{
			}
		})
	}); 
	var viewport=new Ext.Viewport({
		layout:'fit'
		,items:[panelport]
		,listeners:{
			'afterlayout':function(cmp){
				panelport.getTopToolbar().on('initReady',function(tb){
					tb.queryFn(); 
				}); 
			}
		}
	});
	loading_js();
});