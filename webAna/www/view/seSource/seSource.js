Ext.onReady(function(){
	Ext.QuickTips.init();  //表示鼠标移到上面时，显示的信息
	Ext.form.Field.prototype.msgTarget = 'side'; //当有错误时，鼠标移到最后边时才显示错误
	Ext.state.Manager.setProvider(new Ext.state.CookieProvider());//初始化Ext状态管理器，在Cookie中记录用户的操作状态，如果不启用，象刷新时就不会保存当前的状态，而是重新加载 
	Ext.BLANK_IMAGE_URL = '../../js/ext/resources/images/default/s.gif';  //设置背景图片
	
	//同se.js修改一样
	//var monthUrl="../../jsonData/monthData.json";	//月份
	//var weekUrl="../../jsonData/weekData.json";	//周
	var weekUrl="../../controller/getPhases.php?type=week";	//月份
	var monthUrl="../../controller/getPhases.php?type=month";	//月份
	var chartUrl='../../controller/processHostse.php?operation=20';//搜索引擎比例
	var leftGridUrl='../../controller/processHostse.php?operation=21';//搜索关键词排名
	var rightGridUrl='../../controller/processSe.php?operation=22';//关键词网站排名
	
	var dataStore1=new Ext.data.Store({
		proxy: new Ext.data.HttpProxy({url:chartUrl}),	
		reader:new Ext.data.JsonReader({
			totalProperty:'results'
			,root:'rows'
		},new Ext.data.Record.create([
			{name:'word',mapping:'word'}
			,{name:'num',mapping:'num',type:'int'}
		]))
	});
	var dataStore2=new Ext.data.Store({
		proxy: new Ext.data.HttpProxy({url:leftGridUrl}),	
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
		/* ,listeners:{
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
		proxy: new Ext.data.HttpProxy({url:rightGridUrl}),	
		reader:new Ext.data.JsonReader({
			totalProperty:'results'
			,root:'rows'
		},new Ext.data.Record.create([
			{name:'host',mapping:'host'}
			,{name:'num',mapping:'num'}
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
			,leftGridTitle:'搜索关键词来源排名'
			,rightGridTitle:'搜索关键词访问的网站排名'
			,loadMask:true
			,pageSize:13
		}]
		,tbar:new Ext.ux.dateToolbar({
			queryStore:[dataStore1,dataStore2]
			,monthStore:monthStore
			,weekStore:weekStore
			,blankText:'请输入网址'
			//,emptyText:'请输入网址'
			,labelText:'网址:'
			//,regex:/^[a-zA-Z0-9\-\.]+\.(com|org|net|mil|edu|COM|ORG|NET|MIL|EDU)$/ 
			//,regexText:'请输入正确的网址'
			,queryItemId:'host'	//查询textfield id
			,queryItemName:'host'	//查询textfield name
			,buttonText:'搜索趋势'	//查询按钮文字
			,queryItemEnable:true
			,params:{
			}
		})
	}); 
	var viewport=new Ext.Viewport({
		layout:'fit'
		,items:[panelport]
	});
	loading_js();
});