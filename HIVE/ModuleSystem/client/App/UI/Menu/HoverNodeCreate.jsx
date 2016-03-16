import React from "react";
import ReactDOM from "react-dom";
import Core from '../../Core';
import MenuNodeCreate from './MenuNodeCreate.jsx';
import MenuNodeList from './MenuNodeList.jsx';

export default class HoverNodeCreate extends React.Component {
    constructor(props) {
        super(props);

        this.store = this.props.store;
        this.action = this.props.action;
        this.left = 200;
        this.top = 200;

        if(this.props.position && this.props.position.length > 0){
            this.left = this.props.position[0];
            this.top = this.props.position[1];
        }

        this.styles = this.styles.bind(this);
    }

    styles() {
        return {
            menuArea: {
                backgroundColor: "rgba(0, 0, 0, 0.8)",
                border: "3px solid rgb(54, 196, 168)",
                color: "#eee",
                fontSize: "10pt",
                margin: "0px",
                padding: "3px",
                width: "260px",
                height: "260px",
                position: "absolute",
                top: this.top + "px",
                left: this.left + "px",
                zIndex: "9999",
                float: "left"
            }
        };
    }

    render(){
        const style = this.styles();
        return (
            <div style={style.menuArea}>
                <MenuNodeCreate
                    store={this.props.store}
                    action={this.props.action}
                    focusFunction={this.props.focusFunction}
                    hiddenFunction={this.props.hiddenFunction}
                    floating={true}
                    ref="creator"
                />
                <MenuNodeList
                    store={this.props.store}
                    action={this.props.action}
                    hiddenFunction={this.props.hiddenFunction}
                    floating={true}
                />
            </div>
        );
    }
}
