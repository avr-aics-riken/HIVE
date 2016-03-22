import React from "react";
import ReactDOM from "react-dom";
import Core from '../../../Core';

/**
 * ノードプロパティアイテム(TextInput)ビュー.
 */
export default class ItemTextInput extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			value : this.props.initialParam.value,
			onFrame : this.props.store.hasCurrentKeyFrame(this.props.initialParam)
		};
		this.currentEdit = {
			value : null
		};
		this.frameApplied = this.frameApplied.bind(this);
	}

	keyBackGround() {
		if (this.state.onFrame) {
			return "rgb(19, 154, 165)";
		}
		return "white";
	}

	frameApplied(err, content, prop) {
		if (content.nodeVarname === this.props.initialParam.nodeVarname &&
			prop.name === this.props.initialParam.name) {
			if (prop.data.hasOwnProperty(this.props.store.getCurrentFrame())) {
				this.setState({
					onFrame	: true
				});
			} else {
				this.setState({
					onFrame	: false
				});
			}
		}
	}

	componentDidMount() {
		this.props.store.on(Core.Constants.CURRENT_FRAME_APPLIED, this.frameApplied);
	}

	componentWillUnmount() {
		this.props.store.off(Core.Constants.CURRENT_FRAME_APPLIED, this.frameApplied);
	}

	styles() {
        let border = ()=>{
            if(this.props.top && this.props.bottom){
                return {
                    borderRadius: "2px 2px 2px 2px",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }else if(this.props.top){
                return {
                    borderBottom: "1px solid rgb(33, 187, 151)",
                    borderRadius: "2px 2px 0px 0px",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }else if(this.props.bottom){
                return {
                    border: "none",
                    borderRadius: "0px 0px 2px 2px",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }else{
                return {
                    borderBottom: "1px solid rgb(33, 187, 151)",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }
        };
        return {
            view : border.bind(this)(),
            key : {
                backgroundColor: "rgb(84,84,84)",
                color : "white",
                fontSize: "11px",
                letterSpacing: "normal",
                textAlign: this.props.initialParam.name.match(/^\[\d\]$/) ? "right" : "left",
                padding: "1px",
				paddingLeft : "4px",
                width : "80px",
                verticalAlign: "middle",
                display: "inline-block",
                overflow: "hidden",
                textShadow: "0px 0px 3px black"
            },
            value : {
                border: "0px",
				outline: "0",
                borderRadius: "2px",
                color : "#333",
                letterSpacing: "normal",
                marginLeft: "3px",
				marginTop: "1px",
				marginBottom: "1px",
                verticalAlign: "middle",
                padding: "1px",
                width : this.props.filebrowser ? "145px" : "165px",
                height: "19px",
                display: "inline-block",
            },
			addkey : {
				backgroundColor : this.keyBackGround.bind(this)(),
				borderRadius : "6px",
				width : "8px",
				height : "8px",
				marginTop : "6px",
				marginBottom : "6px",
				marginRight : "4px",
				cursor : "pointer",
				position : "absolute"
			}
        };
	}

	onChange() {
		return (ev) => {
			this.currentEdit = {
				value : ev.target.value
			};
			this.setState({
				value : ev.target.value
			});
		};
	}

	submit(ev) {
		if (this.props.initialParam.name === "label" || this.currentEdit.value) {
			if (this.props.initialParam.type === "float") {
				this.props.changeFunc(Number(this.currentEdit.value));
			} else {
				this.props.changeFunc(this.currentEdit.value);
			}
		}
		ev.target.style.border = "none";
		ev.target.blur();
	}

	onKeyPress(ev) {
		if (ev.key === 'Enter') {
			this.submit.bind(this)(ev);
		}
	}

	onBlur(ev) {
		this.submit.bind(this)(ev);
		this.currentEdit = {
			value : null
		};
	}

	onFocus(ev) {
		ev.target.style.border = "2px solid darkgreen";
	}

	onAddKey(ev) {
		if (ev.button === 0) {
			var hole = this.props.initialParam;
			hole.value = this.state.value;
			this.props.changeKeyFunc(hole);
			this.setState({
				onFrame : true
			});
		}
	}

	onDeleteKey(ev) {
		if (ev.button === 2) {
			console.log("delete")
			this.props.deleteKeyFunc(this.props.initialParam);
			this.setState({
				onFrame : false
			});
		}
	}

	addKey() {
		const styles = this.styles.bind(this)();
		if (this.props.initialParam.hasOwnProperty('anim') && !this.props.initialParam.anim) { return; }
		if (this.props.changeKeyFunc !== undefined) {
			return (<span style={styles.addkey} onMouseDown={this.onDeleteKey.bind(this)} onClick={this.onAddKey.bind(this)} />);
		}
	}

	onFilebrowserClick() {
		this.props.openFilebrowserFunc();
	}

	filebrowser() {
		if (this.props.filebrowser) {
			return (
				<span style={{
						position : "absolute",
						width : "18px",
						height : "20px",
						margin : "1px",
						marginLeft : "2px",
						marginRight : "1px",
						borderRadius : "3px",
						cursor : "pointer"}}
					onClick={this.onFilebrowserClick.bind(this)}
				>
					<span style={{
							position : "absolute",
							width : "18px",
							height : "4px",
							top : "2px",
							marginBottom : "2.5px",
							borderRadius : "2px",
							backgroundColor : "white"}}
					/>
					<span style={{
							position : "absolute",
							width : "18px",
							height : "4px",
							top : "8px",
							borderRadius : "2px",
							backgroundColor : "white"}}
					/>
					<span style={{
							position : "absolute",
							width : "18px",
							height : "4px",
							top : "14px",
							borderRadius : "2px",
							backgroundColor : "white"}}
					/>
				</span>);
		}
	}

	render () {
		const styles = this.styles.bind(this)();
		return (<div style={styles.view}>
					<div style={styles.key}>
						{this.addKey.bind(this)()}
						<span style={{marginLeft : "12px"}} title={this.props.initialParam.name}>{this.props.initialParam.name}</span>
					</div>
					<input style={styles.value}
						type="text"
						ref="text"
						value={this.state.value}
						onChange={this.onChange.bind(this)()}
						onKeyPress={this.onKeyPress.bind(this)}
						onBlur={this.onBlur.bind(this)}
						onFocus={this.onFocus.bind(this)}
					>
					</input>
					{this.filebrowser.bind(this)()}
				</div>);
	}
}
