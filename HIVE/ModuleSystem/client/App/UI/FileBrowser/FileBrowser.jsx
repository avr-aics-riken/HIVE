import React from "react";
import ReactDOM from "react-dom";
import Core from '../../Core';


function getUpDir(path) {
	var p,
		uppath,
		i;
	if (path === "/") {
		return "/";
	}
	p = path.split("/");
	if (p[p.length - 1] === "") {
		p.pop();
	}
	uppath = "/";
	for (i = 0; i < p.length - 1; i = i + 1) {
		if (p[i] !== "") {
			uppath += p[i] + '/';
		}
	}
	if (uppath === "//") {
		uppath = "/";
	}
	return uppath;
}

/**
 * ファイルブラウザ
 */
export default class FileBrowser extends React.Component {
	constructor(props) {
		super(props);
		this.store = this.props.store;
		this.action = this.props.action;
		this.state = {
			closeHover : false,
			openHover : false,
			cancelHover : false,
			currentPath : '',
			file : "",
			filter : ""
		};
		this.initialPath = '/';
		this.fileList = [];
	}

	componentDidMount() {
		this.store.on(Core.Constants.INITIALIZED, (err) => {
		 setTimeout(() => {
				this.store.getFileList(this.initialPath, (err, result) => {
					this.fileList = result;
					this.setState({
						currentPath : this.initialPath
					});
				});
			 }, 1000);
		});
	}

	styles() {
		return {
			filebrowser : {
				position : "absolute",
				left : "50%",
				top : "50%",
				transform : "translate(-50%, -50%)",
				width : "505px",
				height : "420px",
				backgroundColor : "rgb(83,83,83)",
				borderRadius : "5px",
				zIndex : "100000",
				display : this.props.display ? "block" : "none"
			},
			title : {
				color : "white",
				fontSize : "16px",
				marginLeft : "12px",
				marginTop : "8px",
				marginBottom : "10px"
			},
			filenameview : {
				width : "242px",
				height : "30px",
				fontSize : "14px",
				border : "2px solid rgb(54, 196, 168)",
				borderRadius : "5px",
				backgroundColor : "white"
			},
			pathview : {
				width : "378px",
				height : "30px",
				paddingTop: "10px",
				paddingLeft : "5px"
			},
			filterview : {
				width : "378px",
				height : "30px",
				border : "2px solid rgb(54, 196, 168)",
				borderRadius : "5px",
				fontSize : "14px",
				backgroundColor : "white",
				marginTop : "5px",
				marginBottom : "6px"
			},
			contentsview : {
				width : "378px",
				height : "268px",
				border : "2px solid rgb(54, 196, 168)",
				borderRadius : "5px",
				backgroundColor : "rgb(50, 50, 50)",
				float : "left",
				overflow : "auto",
				paddingLeft : "5px"
			},
			path : {
				color : "white",
				fontSize : "16px"
			},
			buttonarea : {
				width : "101px",
				height : "268px",
				float : "left"
			},
			buttonareaInner : {
				position : "absolute",
				bottom : "14px",
				right : "4px"
			},
			openbutton : {
				width : "70px",
				height : "27px",
				color : "black",
				margin : "15px",
				backgroundColor : "rgb(54, 196, 168)",
				borderRadius : "5px",
				textAlign : "center",
				padding : "4px",
				opacity : this.state.openHover ? "0.9" : "1.0",
				cursor : "pointer"
			},
			cancelbutton : {
				width : "70px",
				height : "27px",
				color : "white",
				margin : "15px",
				backgroundColor : "rgb(159, 24, 75)",
				borderRadius : "5px",
				textAlign : "center",
				padding : "4px",
				opacity : this.state.cancelHover ? "0.9" : "1.0",
				cursor : "pointer"
			},
			closeButton : {
				position : "absolute",
				right : "0px",
				top : "0px",
				margin : "5px",
				width: "17px",
				height: "17px",
				backgroundColor : "rgb(54, 196, 168)",
				textAlign : "center",
				borderRadius : "4.5px",
				opacity : this.state.closeHover ? "0.9" : "1.0",
				backgroundImage : "url(./img/node_close.png)",
				backgroundRepeat: "no-repeat",
				backgroundSize: "contain",
				cursor : "pointer"
			}
		};
	}

	fileStyle(isDir) {
		if (isDir) {
			return {
				color : "white",
				width : "100%",
				backgroundColor : "rgb(50, 50, 50)"
			}
		} else {
			return {
				color : "white",
				width : "100%",
				backgroundColor : "rgb(50, 50, 50)"
			}
		}
	}

	onItemEnter(ev) {
		ev.target.style.opacity = "0.9";
		ev.target.style.backgroundColor = "rgb(80, 80, 80)";
	}

	onItemLeave(ev) {
		ev.target.style.opacity = "1.0";
		ev.target.style.backgroundColor = "rgb(50, 50, 50)";
	}

	onItemClick(fileobj, index) {
		return (ev) => {
			if (fileobj.type === 'dir') {
				this.store.getFileList(fileobj.path, (err, result) => {
					if (!err) {
						this.fileList = result;
						this.setState({
							currentPath : fileobj.path,
							filter : "",
							file : ""
						});
					}
				});
			} else {
				this.setState({
					file : fileobj.name
				});
			}
		};
	}

	onBackClick(ev) {
		let updir = getUpDir(this.state.currentPath);
		this.store.getFileList(updir, (err, result) => {
			if (!err) {
				this.fileList = result;
				this.setState({
					currentPath : updir,
					filter : "",
					file : ""
				});
			}
	   });
	}

	filelistElem() {
		const style = this.styles();
		let elems = [];
		if (this.state.currentPath && this.state.currentPath !== "/") {
			elems.push(<div key={"back"} style={this.fileStyle(true)}
						onMouseEnter={this.onItemEnter.bind(this)}
						onMouseLeave={this.onItemLeave.bind(this)}
						onClick={this.onBackClick.bind(this)}
						> .. </div>);
		}
		Array.prototype.push.apply(elems,
			this.fileList
				.filter( (a) => { return a.name.indexOf(this.state.filter) >= 0; })
				.filter( (b) => { return b.name[0] !== '.' })
				.map( (fileobj, index) => {
				return (<div key={fileobj.path + "_" + index} style={this.fileStyle(fileobj.type === 'dir')}
							onMouseEnter={this.onItemEnter.bind(this)}
							onMouseLeave={this.onItemLeave.bind(this)}
							onClick={this.onItemClick.bind(this)(fileobj, index)}
							>
							{fileobj.name}
						</div>);
		}));
		return elems;
	}

	/// 閉じるボタンが押された.
	onCloseClick(ev) {
		this.props.action.deleteNode(this.props.nodeVarname);
	}

	/// 閉じるボタンにマウスホバーされた
	onCloseEnter(ev) {
		if (ev.button === 1 || ev.button === 2) { return; }
		this.setState({ closeHover : !this.state.closeHover });
		ev.target.style.cursor = "pointer";
	}

	onCloseLeave(ev) {
		this.setState({ closeHover : !this.state.closeHover });
		ev.target.style.cursor = "default";
	}

	/// キャンセルボタンにマウスホバーされた
	onCancelEnter(ev) {
		if (ev.button === 1 || ev.button === 2) { return; }
		this.setState({ cancelHover : !this.state.cancelHover });
		ev.target.style.cursor = "pointer";
	}

	onCancelLeave(ev) {
		this.setState({ cancelHover : !this.state.cancelHover });
		ev.target.style.cursor = "default";
	}

	/// キャンセルボタンにマウスホバーされた
	onOpenEnter(ev) {
		if (ev.button === 1 || ev.button === 2) { return; }
		this.setState({ openHover : !this.state.openHover });
		ev.target.style.cursor = "pointer";
	}

	onOpenLeave(ev) {
		this.setState({ openHover : !this.state.openHover });
		ev.target.style.cursor = "default";
	}

	onFilterChange(ev) {
		this.setState({
			filter : ev.target.value
		});
	}

	onCancelClick(ev) {
		this.state.file = "";
		this.state.filter = "";
		this.props.cancelFunc();
	}

	onOpenClick(ev) {
		this.state.file = "";
		this.state.filter = "";
		this.props.okFunc(this.state.currentPath + "/" + this.state.file)
	}

	render () {
		const style = this.styles();
		return (<div style={style.filebrowser}
					action={this.props.action}
					store={this.props.store}
				>
					<div style={style.title}>File Browser</div>
					<div style={{paddingLeft : "18px"}}>
						<input type="text" style={style.filenameview} placeholder="file name" value={this.state.file}></input>
						<div style={style.pathview}>
							<div style={style.path}>{this.state.currentPath}</div>
						</div>
						<input style={style.filterview} placeholder="filter" value={this.state.filter} onChange={this.onFilterChange.bind(this)}></input>
						<div style={style.contentwrap}>
							<div style={style.contentsview}>
								{this.filelistElem.bind(this)()}
							</div>
							<div style={style.buttonarea}>
								<div style={style.buttonareaInner}>
									<div style={style.openbutton}
										onClick={this.onOpenClick.bind(this)}
										onMouseEnter={this.onOpenEnter.bind(this)}
										onMouseLeave={this.onOpenLeave.bind(this)} >Open</div>
									<div style={style.cancelbutton}
										onClick={this.onCancelClick.bind(this)}
										onMouseEnter={this.onCancelEnter.bind(this)}
										onMouseLeave={this.onCancelLeave.bind(this)} >Cancel</div>
								</div>
							</div>
						</div>
					</div>
					<div style={style.closeButton}
						onClick={this.onCancelClick.bind(this)}
						onMouseEnter={this.onCloseEnter.bind(this)}
						onMouseLeave={this.onCloseLeave.bind(this)} />
				</div>);
	}
}
