Ext.onReady(function(){
	Ext.QuickTips.init();  //表示鼠标移到上面时，显示的信息
	Ext.form.Field.prototype.msgTarget = 'side'; //当有错误时，鼠标移到最后边时才显示错误
	Ext.state.Manager.setProvider(new Ext.state.CookieProvider());//初始化Ext状态管理器，在Cookie中记录用户的操作状态，如果不启用，象刷新时就不会保存当前的状态，而是重新加载 
	Ext.BLANK_IMAGE_URL = '../../js/ext/resources/images/default/s.gif';  //设置背景图片
	//数据处理
	var makeStore=function(data){
		if($.isArray(data)){
			var firstE=data[0];
			var dataRecords=[];
			for(fieldName in firstE){
				dataRecords[dataRecords.length]={
					"name":fieldName
					,"mapping":fieldName
				}
			}
			return new Ext.data.Store({
				proxy: new Ext.data.MemoryProxy(data),
				reader:new Ext.data.ArrayReader({},new Ext.data.Record.create(dataRecords))
			});
		}
	}
	var makeItem=function(datas){
		var itemsMod=[];
		var itemsStore=[];
		for(name in datas){
			var data=datas[name];
			if($.isArray(data)){
				itemsStore[itemsStore.length]=[name,makeStore(data)];
			}
			else{
				var gridData=data.gridStore;
				var chartData=data.chartStore;
				itemsStore[itemsStore.length]=[name,makeStore(gridData),makeStore(chartData)];
			}
		}
		itemsMod[0]=new Ext.ux.LinechartGrid({
			gridStore:itemsStore[0][1]
			,chartStore:itemsStore[0][1]
			,anchor:'98%'
			,border:true
		});
		for(var i=1;i<itemsStore.length;i++){
			itemsMod[i]=new Ext.ux.PiechartGrid({
				title:itemsStore[i][0]
				,gridStore:itemsStore[i][1]
				,chartStore:itemsStore[i][2]
				//,columnWidth:.25
			});
		}
		return itemsMod;
	}
	var showViewPort=function(itemPs){
		var chartPanel2=new Ext.Panel({
			anchor:'100%'
			,layout:'column'
			//,frame:true
			,border:false
			,defaults:{
				columnWidth:0.49
				,style:'margin:3px;'
				,bodyStyle:'padding:5px 0 5px 5px;'
				,frame:true
				,plain:true
			}
			,items:[itemPs[1],itemPs[2],itemPs[3],itemPs[4]]
		});
		var chartPanel1=new Ext.Panel({
			anchor:'100%'
			,id:'chartPanel1'
			,layout:'anchor'
			//,frame:true
			,plain:true
			,border:false
			,items:[itemPs[0],chartPanel2]
		});

		if(Ext.get('chartPanel1')!=null){
			Ext.get('chartPanel1').remove(); 
		}
		centerPanel.add(chartPanel1);
		centerPanel.doLayout();
	} //eo showViewPort function
	var myMask = new Ext.LoadMask(Ext.getBody(), {msg:"数据加载中，请耐心等待……"});
	var dataLoad=function(store,records,opts){
		myMask.hide();
		var datas=records[0].data;
		var itemPanels=makeItem(datas);
		showViewPort(itemPanels);
	}
	var dataStore = new Ext.data.JsonStore({
        url: '../../testData/keywords.json',
		successProperty:"success",
        root: 'rows',
        fields: [ 
			{name: 'daydata', mapping: 'daydata'},
            {name: 'topdomain', mapping: 'topdomain'},
			{name: 'tophost', mapping: 'tophost'},
            {name: 'topurl', mapping: 'topurl'},
			{name: 'topuserid', mapping: 'topuserid'}
		],
		listeners:{
			beforeload:function(store,opts){
				myMask.show();
			}
			,load:dataLoad
		}
		//,autoLoad:true
    });
	
	var centerPanel=new Ext.Panel({
		layout:'anchor'
		,id:'centerPanel'
		,plain:true
		,autoScroll:true
		,border:false
		,bodyStyle:'background-color:#DFE8F6'
		,tbar:new Ext.ux.dateToolbar({
			queryStore:dataStore
			,queryItemEnable:true
			,blankText:'请输入关键字'
			,emptyText:'请输入关键字'
			,queryItemId:'word'	//查询textfield id
			,queryItemName:'word'	//查询textfield name
			,buttonText:'搜索关键字'	//查询按钮文字
			,params:{}	//附加查询参数
		})
	});
	var viewport=new Ext.Viewport({
		layout:'fit'
		,items:[centerPanel]
	});
	loading_js();
});