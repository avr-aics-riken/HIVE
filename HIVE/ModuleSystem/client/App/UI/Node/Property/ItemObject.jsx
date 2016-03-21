import React from "react";
import ReactDOM from "react-dom";
import Core from '../../../Core';

/**
 * ノードプロパティアイテム(Object)ビュー.
 * プロパティエディタで編集しないけどキーだけ打ちたいなどの場合はこれを使う
 */
export default class ItemObject extends React.Component {
	constructor(props) {
		super(props);

		this.state = {
			onFrame : this.props.hasOwnProperty('store') ? this.props.store.hasCurrentKeyFrame(this.props.initialParam) : false
		};
		this.value = this.props.initialParam.value;
		this.frameApplied = this.frameApplied.bind(this);
	}

	keyBackGround() {
		if (this.state.onFrame) {
			return "rgb(19, 154, 165)";
		}
		return "white";
	}

	frameApplied(err, content, prop) {
		if (this.props.hasOwnProperty('store') && content.nodeVarname === this.props.initialParam.nodeVarname &&
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
		if (this.props.hasOwnProperty('store')) {
			this.props.store.on(Core.Constants.CURRENT_FRAME_APPLIED, this.frameApplied);
		}
	}

	componentWillUnmount() {
		if (this.props.hasOwnProperty('store')) {
			this.props.store.off(Core.Constants.CURRENT_FRAME_APPLIED, this.frameApplied);
		}
	}

    styles() {
        let border = ()=>{
            if(this.props.top){
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
                padding: "1px",
				paddingLeft : "4px",
                width : "80px",
                verticalAlign: "middle",
                display: "inline-block",
                textShadow: "0px 0px 3px black"
            },
            value : {
                color : "#333",
                letterSpacing: "normal",
                marginLeft: "1px",
                padding: "1px",
                width : "20px",
                verticalAlign: "middle",
                display: "inline-block"
            },
			addkey : {
				backgroundColor : this.keyBackGround.bind(this)(),
				borderRadius : "6px",
				width : "8px",
				height : "8px",
				marginTop : "6px",
				marginBottom : "6px",
				marginRight : "4px",
				float : "left",
				cursor : "pointer"
			}
        };
    }

	onAddKey(ev) {
		if (ev.button === 0) {
			var hole = this.props.initialParam;
			hole.value = this.value;
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

	addKeyElem() {
		if (this.props.hasOwnProperty('store')) {
			const styles = this.styles.bind(this)();
			return (<div style={styles.addkey} onMouseDown={this.onDeleteKey.bind(this)} onClick={this.onAddKey.bind(this)} />);
		}
	}

	render () {
		const styles = this.styles.bind(this)();
		return (<div style={styles.view}>
					<div style={styles.key}>
						{this.addKeyElem.bind(this)()}
						{this.props.initialParam.name}
					</div>
					<div style={styles.value}>
						{"(Object)"}
					</div>
				</div>);
	}
}
