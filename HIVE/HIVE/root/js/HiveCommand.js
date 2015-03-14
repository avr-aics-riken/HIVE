/*jslint devel:true*/

(function (window) {
	'use strict';
	var HiveCommand = {
		createCamera : function (name) {
			return 'hcmd.CreateCamera("' + name + '")';
		},
		clearObjects : function () {
			return 'hcmd.ClearObjects()';
		},
		loadOBJ : function (name, filename, shader) {
			return 'return hcmd.LoadOBJ("' + name + '","' + filename + '","' + shader + '")';
		},
		loadSTL : function (name, filename, shader) {
			return 'return hcmd.LoadSTL("' + name + '","' + filename + '","' + shader + '")';
		},
		loadPDB : function (name, filename, shader) {
			return 'return hcmd.LoadPDB("' + name + '","' + filename + '","' + shader + '")';
		},
		loadSPH : function (name, filename, shader) {
			return 'return hcmd.LoadSPH("' + name + '","' + filename + '","' + shader + '")';
		},
		renderCamera : function (w, h, cameraname) {
			return 'return hcmd.RenderCamera(' + w + ',' + h + ',"' + cameraname + '")';
		},
		newScene : function () {
			return 'hcmd.NewScene()';
		},
		deleteObject : function (name) {
			return 'hcmd.DeleteObject("' + name + '")';
		},
		updateSceneInformation : function () {
			return 'hcmd.UpdateSceneInformation()';
		},
		cameraScreenSize : function (name, width, height) {
			return 'hcmd.CameraScreenSize("' + name + '",' + width + ',' + height + ')';
		},
		cameraPos : function (name, camerapos) {
			console.log('Duplicated method.');
			return 'hcmd.CameraPos("' + name + '",' + camerapos[0] + ',' + camerapos[1] + ',' + camerapos[2] + ')';
		},
		cameraLookat : function (name, camera_pos, camera_tar, camera_up, camera_fov) {
			var src = 'hcmd.CameraLookat("';
			src += name + '",';
			src += camera_pos[0] + ',' + camera_pos[1] + ',' + camera_pos[2] + ',';
			src += camera_tar[0] + ',' + camera_tar[1] + ',' + camera_tar[2] + ',';
			src += camera_up[0]  + ',' + camera_up[1]  + ',' + camera_up[2]  + ',';
			src += camera_fov;
			src += ')';
			return src;
		}
	};
	window.HiveCommand = HiveCommand;
}(window));