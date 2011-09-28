//loading的js方法
function loading_js() {
	setTimeout(function(){
        Ext.get('loading').remove();
        Ext.get('loading-mask').fadeOut({remove:true});
    }, 250);
}

//当是非法用户时提示的错误信息,并提供按钮跳转到登陆页面
function invalid_login_js(ext_image_url,location_login,height,width){
    height=parseInt(height); //将string转化为int 
	width=parseInt(width);//将string转化为int 
	Ext.onReady(function(){     
	    Ext.QuickTips.init();  //表示当出错时，显示的错误信息    
	   //Ext.BLANK_IMAGE_URL ='ext/resources/images/default/s.gif';
	    Ext.BLANK_IMAGE_URL =ext_image_url;
	    Ext.form.Field.prototype.msgTarget = 'side';  
	     Ext.Msg.show({
	     title:'非法登陆',
	    //defaultTextHeight:20,
	     defaultTextHeight:height,
	     draggable:false,
	     closable:false,
	    //width:365,
	     width:width,
	     msg:'对不起,您还没有登陆本系统!请按确定按钮登陆本系统',
	     buttons:Ext.Msg.OK,
	     icon: Ext.Msg.WARNING,
	     fn:function(bn){
	          document.location.href=location_login;  //跳转到登陆页面        
	         }
	   });            
	});
}




function common_submit(panel,url_path,success_location){ 
    if(panel.form.isValid()){//验证合法后使用加载进度条
	      Ext.MessageBox.show({
			   title: '请稍等',
			   msg: '正在处理......',
			   progressText: '',
			   width:300,
			   progress:true,
			   closable:false,
			   animEl: 'loading'  //绑定动画的元素名称
		   });
		   //控制进度速度
		   var f = function(v){
			 return function(){
						var i = v/11;
						  Ext.MessageBox.updateProgress(i,Math.round(100*i)+"% completed",'');
		            };
		   };

		   for(var i = 1; i < 13; i++){
			   setTimeout(f(i), i*200);  //设置i*200秒执行一次
		   }
        
		  //提交到服务器操作
		   panel.form.doAction('submit',{
			 url:url_path,//相对路径文件路径,取与调用改js文件的php文件的相对路径
			 method:'post',//提交方法post或get
			 params:'',
			 //提交成功的回调函数
			 success:function(form,action){
					if (action.result.msg=='ok') {
						 document.location=success_location;
					} else {
						//Ext.Msg.alert('登陆错误',action.result.msg);
						Ext.Msg.show({
							 title:'登陆失败',
							 defaultTextHeight:20,
					         draggable:false,
					         closable:false,
					         width :200,
					         msg:action.result.msg,
					         buttons:Ext.Msg.OK,
					         icon: Ext.Msg.ERROR 
							
						});
					}
			 },
			 //提交失败的回调函数
			 failure:function(){
					Ext.Msg.alert('错误','服务器出现错误请稍后再试！');
			 }
		  });  // }) 也可以	 
	   }   
}


function getIds(grid2) {
	var s = grid2.getSelectionModel().getSelections();
	if (s.length==0) {
		return null;
	}
	var ids = [];
	for (i=0;i<s.length;i++) {
		ids.push(s[i].id);
	}
	ids = ids.join(',');
	return ids;
}
function getId(grid2) {
	var s = grid2.getSelectionModel().getSelected();
	if (s) {
		return s.id;
	}
	return 0;
}