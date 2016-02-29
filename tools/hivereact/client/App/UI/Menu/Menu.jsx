import React from "react";
import ReactDOM from "react-dom";
import Core from '../../Core';
import MenuStore from './MenuStore.jsx';
import MenuNodeCreate from './MenuNodeCreate.jsx';
import MenuNodeList from './MenuNodeList.jsx';

export default class Menu extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            nodes: null,
            visible: this.props.menuStore.getVisible()
        };

        this.store = this.props.store;
        this.action = this.props.action;
        this.layoutType = this.props.layoutType;
        this.menuStore = this.props.menuStore;
        this.menuAction = this.props.menuAction;
        this.styles = this.styles.bind(this);
        this.onClick = this.onClick.bind(this);
        this.props.menuStore.on(MenuStore.TOGGLE_MENU, this.toggleMenu.bind(this));
    }

    onClick(eve){
        let f = this.menuStore.getVisible();
        this.props.menuAction.toggleMenu(!f);
    }

    toggleMenu(){
        if(this.layoutType === 1 || this.layoutType === 3){return;}
        let f = this.menuStore.getVisible();
        this.setState({visible: f});
    }

    styles() {
        return {
            menuArea: {
                backgroundColor: "rgba(96, 16, 8, 0.5)",
                color: "#eee",
                fontSize: "10pt",
                margin: "0px",
                padding: "0px",
                width: "200px",
                height: "100%",
                position: this.layoutType !== 2 ? "fixed" : "static",
                top: "0px",
                left: this.state.visible ? "0px" : "-200px",
                zIndex: "9999",
                transition: "left 0.5s cubic-bezier(0.18, 0.93, 0.26, 0.99)",
                float: "left",
                overflow: "auto" // temp
            },
            header: {
                backgroundColor: "rgba(200, 200, 200, 0.7)",
                textAlign: "30px",
                margin: "0px",
                padding: "0px",
                width: "100%",
                minHeight: "30px",
                boxShadow: "0px 0px 1px 0px white inset"
            },
            headerCaption: {
                color: "#333",
                fontWeight: "bold",
                padding: "5px 10px",
                textShadow: "0px 0px 1px #fab",
                boxShadow: "-1px -1px 0px 1px #666 inset"
            },
            container: {
                margin: "0px",
                padding: "0px",
                width: "200px",
                position: "relative",
                // left: this.state.visible ? "0px" : "-200px",
                // transition: "left 0.5s cubic-bezier(0.18, 0.93, 0.26, 0.99)"
            }
        };
    }

    render(){
        const style = this.styles();
        return (
            <div style={style.menuArea}>
                <div style={style.container}>
                    <div style={style.header}>
                        <div style={style.headerCaption}>Node Create</div>
                    </div>
                    <MenuNodeCreate
                        store={this.props.store}
                        action={this.props.action}
                        menuStore={this.props.menuStore}
                        menuAction={this.props.menuAction}
                    />
                    <MenuNodeList
                        store={this.props.store}
                        action={this.props.action}
                        menuStore={this.props.menuStore}
                        menuAction={this.props.menuAction}
                    />
                </div>
            </div>
        );
    }
}

