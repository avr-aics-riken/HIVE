import React from "react";
import ReactDOM from "react-dom";
import Core from '../../Core';

/**
 * ファイルブラウザ
 */
export default class LabelDialog extends React.Component {
	constructor(props) {
		super(props);
		this.store = this.props.store;
		this.action = this.props.action;
		this.state = {
			openHover : false,
			closeHover :false,
			cancelHover : false,
			label : ""
		};
	}

	styles() {
		return {
			lock : {
				backgroundColor: "rgba(0, 0, 0, 0.1)",
				zIndex: "99999",
				left: "0",
				top: "0",
				width: "100%",
				minHeight: "100%",
				minWidth: "800px",
				height: "auto",
				position: "fixed",
				display: this.props.display ? "block" : "none"
			},
			LabelDialog : {
				position : "absolute",
				left : "50%",
				top : "50%",
				transform : "translate(-50%, -50%)",
				width : "410px",
				height : "120px",
				paddingBottom : "12px",
				backgroundColor : "rgb(83,83,83)",
				borderRadius : "5px",
				zIndex : "100000",
				display : this.props.display ? "block" : "none",
				paddingTop : "10px",
				paddingLeft : "10px"
			},
			okButton : {
				width : "70px",
				height : "27px",
				color : "black",
				margin : "15px",
				backgroundColor : "rgb(54, 196, 168)",
				borderRadius : "5px",
				textAlign : "center",
				padding : "4px",
				opacity : this.state.openHover ? "0.9" : "1.0",
				cursor : "pointer",
				right : "80px",
				position : "absolute"
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
				cursor : "pointer",
				right : "0px",
				position : "absolute"
			},
			labelview : {
				width : "378px",
				height : "30px",
				border : "2px solid rgb(54, 196, 168)",
				borderRadius : "5px",
				fontSize : "14px",
				backgroundColor : "white",
				marginTop : "5px",
				marginBottom : "6px"
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
			},
			title : {
				color : "white",
				fontSize : "16px"
			}
		};
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

	onLabelChange(ev) {
		this.setState({
			label : ev.target.value
		});
	}

	onCancelClick(ev) {
		this.props.cancelFunc();
		this.state.label = "";
	}

	onOpenClick(ev) {
		this.props.okFunc(this.state.label)
		this.state.label = "";
	}

	render () {
		const style = this.styles();
		return (<div>
					<div style={style.LabelDialog}>
						<div style={style.title}>Label Setting</div>
						<div>
							<input type="text" style={style.labelview} placeholder="label" value={this.state.label} onChange={this.onLabelChange.bind(this)} />
							<div style={style.buttonarea}>
								<div style={style.buttonareaInner}>
									<div style={style.okButton}
										onClick={this.onOpenClick.bind(this)}
										onMouseEnter={this.onOpenEnter.bind(this)}
										onMouseLeave={this.onOpenLeave.bind(this)} >OK</div>
									<div style={style.cancelbutton}
										onClick={this.onCancelClick.bind(this)}
										onMouseEnter={this.onCancelEnter.bind(this)}
										onMouseLeave={this.onCancelLeave.bind(this)} >Cancel</div>
								</div>
							</div>
						</div>
						<div style={style.closeButton}
							onClick={this.onCancelClick.bind(this)}
							onMouseEnter={this.onCloseEnter.bind(this)}
							onMouseLeave={this.onCloseLeave.bind(this)} />
					</div>
					<div style={style.lock}  />
				</div>);
	}
}
