<template>
  <div class="container">
    <h1 class="is-size-1 has-text-centered">进程调度</h1>

    <div class="options">
      <h2 class="is-size-2">参数配置</h2>
      <h3 class="is-size-3">进程调度方法选择</h3>
      <div class="methods control radio-group">
        <label class="radio" for="static">
          <input type="radio" v-model="type" id="static" value="static"> 静态优先级调度
        </label>
        <label class="radio" for="dynamic">
          <input type="radio" v-model="type" id="dynamic" value="dynamic"> 动态优先级调度
        </label>
        <label class="radio" for="round">
          <input type="radio" v-model="type" id="round" value="round"> 时间片轮转调度
        </label>
      </div>

      <h3 class="is-size-3">进程名与所需时间</h3>
      <div class="settings">
        <InputGroup ref="input"/>
      </div>

      <div class="control">
        <a class="button is-info is-rounded" @click="handleClick">开始调度</a>
      </div>
    </div>

    <h2 class="is-size-2">处理过程</h2>
    <div class="process">
      <ProcessControl ref="control"/>
    </div>

  </div>
</template>

<script>
import ProcessControl from "./components/ProcessControl";
import InputGroup from "./components/InputGroup";
import Process from "./class/Process";

export default {
  name: "App",
  components: {
    ProcessControl,
    InputGroup
  },

  data() {
    return {
      // 给select赋默认值static
      type: "static"
    };
  },

  methods: {
    // 处理调度点击
    handleClick: function() {
      let processArr = [];
      // 获取子组件内容
      let inputArr = this.$refs.input.getValues();
      inputArr.map(input =>
        processArr.push(new Process(input.name, Number(input.value)))
      );
      // 调用子组件方法
      this.$refs.control.init(processArr, this.type);
    }
  }
};
</script>

<style>
h1,
h2,
h3,
h4,
h5,
div {
  margin-bottom: 20px;
}

.input-name {
  width: 30%;
  margin-right: 10px;
}

.container {
  padding: 10px;
}

.radio-group {
  display: flex;
  flex-direction: column;
}

.radio {
  margin-left: .5em;
}
</style>
