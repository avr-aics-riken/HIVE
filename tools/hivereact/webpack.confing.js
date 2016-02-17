var path    = require('path');
var webpack = require('webpack');
var NodeTargetPlugin = require('webpack/lib/node/NodeTargetPlugin')
var ExternalsPlugin = webpack.ExternalsPlugin

module.exports = {
    entry: './client/app.jsx',
    output: {path: __dirname, filename: 'client/bundle.js'},
    module: {
        loaders: [
            {
                test: /.jsx?$/,
                loader: 'babel-loader',
                exclude: /node_modules/
            }
        ]
    },
    devServer: {
        contentBase: "./client",
        port: 8081,
        hot: true,
        inline: true,
    },
	plugins: [
		new webpack.NoErrorsPlugin(),
		new webpack.ExternalsPlugin('commonjs', [
        'buffercopy',
        'nanomsg',
        'electron',
		'app',
		'auto-updater',
		'browser-window',
		'content-tracing',
		'dialog',
		'global-shortcut',
		'ipc',
        'ipcRenderer',
        'ipcMain',
		'menu',
		'menu-item',
		'power-monitor',
		'protocol',
		'tray',
		'remote',
		'web-frame',
		'clipboard',
		'crash-reporter',
		'screen',
		'shell'
		]),
		new NodeTargetPlugin(),
        new webpack.HotModuleReplacementPlugin()
	]
}

