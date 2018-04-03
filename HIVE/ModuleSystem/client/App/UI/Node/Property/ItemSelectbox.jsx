import React from "react";
import ReactDOM from "react-dom";
import Core from '../../../Core';

/**
 * ノードプロパティアイテム(Selectbox)ビュー.
 */
export default class ItemSelectbox extends React.Component {
	constructor(props) {
		super(props);

		this.state = {
			value : this.props.initialParam.value,
			onFrame : this.props.hasOwnProperty('store') ? this.props.store.hasCurrentKeyFrame(this.props.initialParam) : false
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
                marginLeft: "5px",
                padding: "1px",
                width : "160px",
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
				cursor : "pointer",
				position : "absolute"
			}
        };
    }

	onChange(ev) {
		this.props.changeSelectboxFunc(ev.target.value);
		this.setState({
			value : ev.target.value
		});
	}

	onChangeKey(ev) {
		if (ev.button === 0 || ev.button === 2) {
			var hole = this.props.initialParam;
			hole.value = this.state.value;
			if (!this.state.onFrame === false) {
				this.props.deleteKeyFunc(this.props.initialParam);
			} else {
				this.props.changeKeyFunc(hole);
			}
			this.setState({
				onFrame : !this.state.onFrame
			});
		}
	}

	addKeyElem() {
		if (this.props.hasOwnProperty('store')) {
			if (this.props.initialParam.hasOwnProperty('anim') && !this.props.initialParam.anim) { return; }
			const styles = this.styles.bind(this)();
			return (<span style={styles.addkey} onMouseDown={this.onChangeKey.bind(this)} />);
		}
	}
	
	options() {
		let initialParam = this.props.initialParam;
		let res = initialParam.option.keys.map(function (optionKey, key) {
			let value = initialParam.option.values[key];
			return (<option 
				value={value} 
				key={initialParam.nodeVarname + "_" + initialParam.name + "_" + key}>{optionKey}</option>);
		});
		return res;
	}

	render () {
		const styles = this.styles.bind(this)();
		return (<div style={styles.view}>
					<div style={styles.key}>
						{this.addKeyElem.bind(this)()}
						<span style={{marginLeft : "12px"}} title={this.props.initialParam.name}>{this.props.initialParam.name}</span>
					</div>
					<select
						value={this.state.value}
						style={styles.value}
						onChange={this.onChange.bind(this)}
					>
						{this.options.bind(this)()}
					</select>
				</div>);
	}
}
