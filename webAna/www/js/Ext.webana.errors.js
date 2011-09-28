if(typeof(Ext.webana)=="undefined"){
	Ext.ns('Ext.webana');
}
Ext.webana.errors={};

errors.loadexceptionFn=function(othis, options, response, e){
	var errorInfo=Ext.util.JSON.decode(response.responseText);
	if(errorInfo.success==false){
		switch(errorInfo.results){
			case 97:
				Ext.Msg.alert('提示','操作成功');
				break;
			case 98:
				Ext.Msg.alert('提示','操作失败！');
				break;
			case 120:
				Ext.Msg.alert('提示','参数错误！');
				break;
			case 99:
				Ext.Msg.alert('提示','服务器错误！');
				break;
			default:
				Ext.Msg.alert('提示','其它错误！');
				break;
		}
	}
}
