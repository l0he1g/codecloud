Ext.onReady(function(){
	Ext.QuickTips.init();  //表示鼠标移到上面时，显示的信息
	Ext.form.Field.prototype.msgTarget = 'side'; //当有错误时，鼠标移到最后边时才显示错误
	Ext.state.Manager.setProvider(new Ext.state.CookieProvider());//初始化Ext状态管理器，在Cookie中记录用户的操作状态，如果不启用，象刷新时就不会保存当前的状态，而是重新加载 
	Ext.BLANK_IMAGE_URL = '../../js/ext/resources/images/default/s.gif';  //设置背景图片
	
	var loginForm = new Ext.form.FormPanel({
		labelWidth: 40, 			
		baseCls: 'x-plain',
		autoHeight:true,
		labelAlign:'right', //label的对齐方式,默认的是向左对齐
		defaults: {width:200},
		frame:true,
		defaultType: 'textfield',//默认字段类型
		//定义表单元素
		items: [{
				fieldLabel: '用户名',
				name: 'username',//元素名称
				allowBlank:false,//不允许为空
				id:"userID",
				blankText:'用户名不能为空',//错误提示内容
				regex:/^[a-zA-Z0-9-]{4,250}$/,
				regexText:'至少6位字符，可以使用大小写英文字母、阿拉伯数字以及中划线"-",不允许其它特殊字符'
			},{
				inputType:'password',
				fieldLabel: '密码',
				name: 'password',
				id:'password',
				allowBlank:false,
				blankText:'密码不能为空', //错误提示内容
				regex:/^[a-zA-Z0-9]{6,40}$/,
				regexText:'只能为6位或6位以上的大小写字母或阿拉伯数字，不能为其它字符'
			}
		],
		buttonAlign:'center',
		buttons: [{
			text: '登录',
			type: 'submit',
			//定义表单提交事件  有两种方式 1.使用fn:方法名 2.使用handler:function(){...}来处理相应的事件
			handler:do_submit
		},{
			text: '重置',
			handler:function(){
			  loginForm.form.reset();
			  //win.hide();
			}//重置表单
		}],
		 //增加键盘回车事件  
		keys:[{
			key:Ext.EventObject.ENTER,   
			fn:do_submit,   
			scope:this  
		}] 
	});    

		//add by liujun   添加do_submit函数,用来处理回车提交表单
	function do_submit(){ 
		if(!loginForm.form.isValid()){
			Ext.Msg.alert("提示","用户名或密码格式不正确");
			return false;
		}
	   else{//验证合法后使用加载进度条
			 Ext.MessageBox.show({
				   title: '请稍等',
				   msg: '正在处理......',
				   progressText: '',
				   width:300,
				   progress:true,
				   closable:false,
				   animEl: 'loading'   //绑定动画的元素名称
			   });
			   //控制进度速度
			  var f = function(v){
				 return function(){
							var i = v/11;
							 //Ext.MessageBox.updateProgress(i, '');
							  Ext.MessageBox.updateProgress(i,Math.round(100*i)+"% completed",'');
						};
			   };

			   for(var i = 1; i < 12; i++){
					//setTimeout(f(i), i*500);
				   setTimeout(f(i), i*200);  //设置i*200秒执行一次
			   }
			
			  //提交到服务器操作
			  loginForm.form.doAction('submit',{
				 url:'check.php', //提交后台处理的php地址
				 method:'post',//提交方法post或get
				 params:{},
				 //提交成功的回调函数
				success:function(form,action){
					var result=Ext.util.JSON.decode(action.response.responseText);
					window.location="../../index.php";
				},
				 //提交失败的回调函数
				failure:function(form,action){
					if(action.failureType === Ext.form.Action.SERVER_INVALID)
					{
						Ext.Msg.alert("提示",action.result.errors.msg);
					}
					else{
						Ext.Msg.alert("提示","用户名或密码错误!");
					}
					 
				}
			  });  
		}   //eo else
		
	} //eo do_submit()
	  
	//定义窗体
	var win_login = new Ext.Window({
		id:'win_login',
		title:'用户登录',
		layout:'fit',	//之前提到的布局方式fit，自适应布局					
		width:300,
		height:130,
		bodyStyle:'padding:5px;',
		maximizable:false,//禁止最大化
		resizable:false, //禁止拉缩
		closeAction:'close',
		closable:false,//禁止关闭
		collapsible:true,//可折叠
		draggable:false, //禁止拖动	
		constrain:true,					
		buttonAlign:'center',
		items:loginForm//将表单作为窗体元素嵌套布局
	});
		
	win_login.show();//显示窗体
	
	var center = new Ext.Panel({
		region:'center',
		layout:'fit',
        frame:true,
		plain:true
    });
	var south=new Ext.Panel({
		region:'south'
		,frame:true
		,plain:true
		,height:80
		,collapsible:true
		,html:'<div class="footer">CopyRight</div>'
	});
	var viewport=new Ext.Viewport({
		layout:'border'
		,items:[center]
	});
	loading_js();
});